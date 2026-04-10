#!/usr/bin/env python3
"""
main.cpp アセンブラ

src/ ディレクトリ内の .hpp ファイルを番号順に結合して main.cpp を生成する。
競プロではペライチ（単一ファイル）が便利なため、開発は src/ で分割し、
提出用の main.cpp は本スクリプトで自動生成する。

使い方:
  python3 tools/assemble.py          # main.cpp を生成
  python3 tools/assemble.py --check  # main.cpp が最新か確認（CI用）
"""

import sys
from pathlib import Path


def assemble(src_dir: Path) -> str:
    """src/ 内の .hpp ファイルを番号順に結合して返す"""
    files = sorted(src_dir.glob("*.hpp"))
    if not files:
        print("Error: no .hpp files found in src/", file=sys.stderr)
        sys.exit(1)

    parts = []
    for f in files:
        content = f.read_text()
        # 末尾の空行を正規化
        parts.append(content.rstrip())

    return "\n\n".join(parts) + "\n"


def main():
    project_root = Path(__file__).resolve().parent.parent
    src_dir = project_root / "src"
    output_path = project_root / "main.cpp"

    if not src_dir.exists():
        print(f"Error: {src_dir} not found", file=sys.stderr)
        sys.exit(1)

    assembled = assemble(src_dir)
    files = sorted(src_dir.glob("*.hpp"))

    if "--check" in sys.argv:
        if not output_path.exists():
            print("main.cpp does not exist", file=sys.stderr)
            sys.exit(1)
        current = output_path.read_text()
        if current == assembled:
            print(f"main.cpp is up-to-date ({len(files)} files)")
            sys.exit(0)
        else:
            print("main.cpp is out-of-date. Run: python3 tools/assemble.py", file=sys.stderr)
            sys.exit(1)

    output_path.write_text(assembled)
    print(f"Assembled main.cpp from {len(files)} files")


if __name__ == "__main__":
    main()
