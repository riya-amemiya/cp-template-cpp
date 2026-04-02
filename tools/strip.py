#!/usr/bin/env python3
"""
提出用コードストリッパー

main.cpp のテンプレートからユーザーの解答コードが使っていない
ブロック(namespace, struct, 関数群)を除去し、最小限のコードを生成する。

使い方:
  python3 tools/strip.py solution.cpp > submission.cpp
  python3 tools/strip.py < solution.cpp > submission.cpp

main.cpp 内の // @begin <name> / // @end <name> マーカーで区切られた
ブロックを管理し、// @dep <name> で依存関係を宣言する。
マーカーのないコード (型エイリアス、定数、マクロなど) は常に含まれる。

// @exports <id1> <id2> ... でブロックが公開する識別子を明示できる。
明示されていない場合はブロック名自体が識別子として使われる。
"""

import re
import sys
from pathlib import Path


def parse_template(template_path: str) -> tuple[str, list[dict]]:
    """main.cpp をパースして、常に含める部分とブロック群を返す"""
    lines = Path(template_path).read_text().splitlines()

    always_lines = []
    blocks = []
    current_block = None
    in_main = False

    for line in lines:
        # @begin マーカー
        m = re.match(r"^\s*//\s*@begin\s+(\S+)", line)
        if m:
            current_block = {
                "name": m.group(1),
                "lines": [line],
                "deps": [],
                "exports": [],
            }
            continue

        # @end マーカー
        m = re.match(r"^\s*//\s*@end\s+(\S+)", line)
        if m:
            if current_block:
                current_block["lines"].append(line)
                blocks.append(current_block)
                current_block = None
            continue

        # @dep マーカー
        m = re.match(r"^\s*//\s*@dep\s+(\S+)", line)
        if m and current_block:
            current_block["deps"].append(m.group(1))
            current_block["lines"].append(line)
            continue

        # @exports マーカー
        m = re.match(r"^\s*//\s*@exports\s+(.+)", line)
        if m and current_block:
            current_block["exports"].extend(m.group(1).split())
            current_block["lines"].append(line)
            continue

        # #ifndef TESTING のメイン関数ガードをスキップ
        if re.match(r"^\s*#ifndef\s+TESTING", line):
            in_main = True
            continue
        if in_main:
            if re.match(r"^\s*#endif", line):
                in_main = False
            continue

        if current_block:
            current_block["lines"].append(line)
        else:
            always_lines.append(line)

    return "\n".join(always_lines), blocks


def find_needed_blocks(
    solution_code: str, blocks: list[dict]
) -> set[str]:
    """解答コードが使用しているブロックを特定し、依存関係を解決する"""
    # ブロック名 -> ブロック のマップ
    block_map = {b["name"]: b for b in blocks}

    # 各ブロックが公開する識別子を収集
    name_to_block = {}
    for b in blocks:
        exports = b["exports"] if b["exports"] else [b["name"].split("::")[-1]]
        for ident in exports:
            name_to_block[ident] = b["name"]

    # 解答コード中の識別子を検索
    needed = set()
    for ident, block_name in name_to_block.items():
        # 識別子が解答コードに出現するかチェック (単語境界)
        if re.search(r"\b" + re.escape(ident) + r"\b", solution_code):
            needed.add(block_name)

    # 依存関係の推移閉包
    resolved = set()
    queue = list(needed)
    while queue:
        name = queue.pop()
        if name in resolved:
            continue
        resolved.add(name)
        if name in block_map:
            for dep in block_map[name]["deps"]:
                if dep not in resolved:
                    queue.append(dep)

    return resolved


def strip_markers(text: str) -> str:
    """@begin/@end/@dep/@exports マーカーコメントを除去"""
    lines = text.splitlines()
    result = []
    for line in lines:
        if re.match(r"^\s*//\s*@(begin|end|dep|exports)\s", line):
            continue
        result.append(line)
    return "\n".join(result)


def main():
    # main.cpp のパス (スクリプトの2階層上)
    script_dir = Path(__file__).resolve().parent
    template_path = script_dir.parent / "main.cpp"

    if not template_path.exists():
        print(f"Error: {template_path} not found", file=sys.stderr)
        sys.exit(1)

    # 解答コードを読み込み
    if len(sys.argv) > 1:
        solution_path = sys.argv[1]
        solution_code = Path(solution_path).read_text()
    else:
        solution_code = sys.stdin.read()

    # テンプレートをパース
    always_code, blocks = parse_template(str(template_path))

    # 解答コードから main() 関数部分を抽出 (あれば)
    # main() 以外のコードも含めてマッチング対象にする
    scan_code = solution_code

    # 必要なブロックを特定
    needed = find_needed_blocks(scan_code, blocks)

    # 出力を組み立て
    output_parts = [always_code.rstrip()]

    # ブロックを元の順序で出力
    for b in blocks:
        if b["name"] in needed:
            block_text = "\n".join(b["lines"])
            output_parts.append(block_text)

    # 解答コードの main() を追加
    output_parts.append(solution_code.rstrip())

    output = "\n\n".join(output_parts) + "\n"

    # マーカーコメントを除去
    output = strip_markers(output)

    # 連続する空行を最大2行に圧縮
    output = re.sub(r"\n{3,}", "\n\n", output)

    print(output, end="")


if __name__ == "__main__":
    main()
