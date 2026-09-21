#!/usr/bin/env python3
"""
提出用コードストリッパー

解答が参照していない宣言（using / 定数 / マクロ / 関数 / struct / namespace 内の要素）
を削除し、文字数制限向けの最小コードを出力する。

使い方:
  python3 tools/strip.py                  # main.cpp をツリーシェイク
  python3 tools/strip.py solution.cpp     # 完全なソースをツリーシェイク
  python3 tools/strip.py snippet.cpp -o submission.cpp
      # #include / main を含まない断片は main.cpp の main() に挿入してから削減

// @begin / @end / @dep / @exports は出力から除去する。
@exports はその直後の宣言が公開する識別子の追加ヒント、
@dep は追加の依存識別子として扱う。
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path


DEFAULT_MACROS = {"LOCAL": False, "TESTING": False}

KEYWORDS = {
    "alignas",
    "alignof",
    "and",
    "and_eq",
    "asm",
    "auto",
    "bitand",
    "bitor",
    "bool",
    "break",
    "case",
    "catch",
    "char",
    "char8_t",
    "char16_t",
    "char32_t",
    "class",
    "compl",
    "concept",
    "const",
    "consteval",
    "constexpr",
    "constinit",
    "const_cast",
    "continue",
    "co_await",
    "co_return",
    "co_yield",
    "decltype",
    "default",
    "delete",
    "do",
    "double",
    "dynamic_cast",
    "else",
    "enum",
    "explicit",
    "export",
    "extern",
    "false",
    "float",
    "for",
    "friend",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "mutable",
    "namespace",
    "new",
    "noexcept",
    "not",
    "not_eq",
    "nullptr",
    "operator",
    "or",
    "or_eq",
    "private",
    "protected",
    "public",
    "register",
    "reinterpret_cast",
    "requires",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "static_assert",
    "static_cast",
    "struct",
    "switch",
    "template",
    "this",
    "thread_local",
    "throw",
    "true",
    "try",
    "typedef",
    "typeid",
    "typename",
    "union",
    "unsigned",
    "using",
    "virtual",
    "void",
    "volatile",
    "wchar_t",
    "while",
    "xor",
    "xor_eq",
    "override",
    "final",
    "std",
    "size_t",
    "int8_t",
    "int16_t",
    "int32_t",
    "int64_t",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",
    "__VA_ARGS__",
    "__VA_OPT__",
    "__int128",
    "__uint128_t",
    "__builtin_clz",
    "__builtin_clzll",
    "__builtin_ctz",
    "__builtin_ctzll",
    "__builtin_popcount",
    "__builtin_popcountll",
}

IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
MARKER_RE = re.compile(r"^\s*//\s*@(begin|end|dep|exports)\b")


@dataclass
class Unit:
    text: str
    defined: set[str]
    used: set[str]
    ns: tuple[str, ...] = ()
    keep_always: bool = False
    is_root: bool = False


def mask_comments_and_strings(text: str) -> str:
    """コメントと文字列リテラルを空白に置換して識別子抽出用にする。"""
    out = []
    i = 0
    n = len(text)
    while i < n:
        c = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if c == "/" and nxt == "/":
            while i < n and text[i] != "\n":
                out.append(" ")
                i += 1
            continue
        if c == "/" and nxt == "*":
            out.append("  ")
            i += 2
            while i < n and not (text[i] == "*" and i + 1 < n and text[i + 1] == "/"):
                out.append("\n" if text[i] == "\n" else " ")
                i += 1
            if i < n:
                out.append("  ")
                i += 2
            continue
        if c in "\"'":
            quote = c
            out.append(" ")
            i += 1
            while i < n:
                ch = text[i]
                if ch == "\\":
                    out.append("  ")
                    i += 2
                    continue
                if ch == quote:
                    out.append(" ")
                    i += 1
                    break
                out.append("\n" if ch == "\n" else " ")
                i += 1
            continue
        if c == "R" and nxt == '"':
            # raw string R"delim(...)delim"
            j = i + 2
            while j < n and text[j] != "(":
                j += 1
            delim = text[i + 2 : j]
            end = f"){delim}\""
            k = text.find(end, j + 1)
            if k < 0:
                out.append(" " * (n - i))
                break
            chunk = text[i : k + len(end)]
            out.append("".join("\n" if ch == "\n" else " " for ch in chunk))
            i = k + len(end)
            continue
        out.append(c)
        i += 1
    return "".join(out)


def extract_idents(text: str) -> set[str]:
    masked = mask_comments_and_strings(text)
    idents = set()
    for m in IDENT_RE.finditer(masked):
        name = m.group(0)
        if name in KEYWORDS or name.startswith("__"):
            continue
        idents.add(name)
    return idents


def expand_conditionals(text: str, macros: dict[str, bool] | None = None) -> str:
    """既知の #ifdef / #ifndef を展開する。未知のマクロはブロックごと残す。"""
    macros = macros or DEFAULT_MACROS
    lines = text.splitlines(keepends=True)
    out: list[str] = []
    # taking: this branch is active; parent_taking: ancestors allow emission
    stack: list[tuple[bool, bool]] = []

    def emitting() -> bool:
        return all(frame[0] for frame in stack) if stack else True

    i = 0
    while i < len(lines):
        raw = lines[i]
        stripped = raw.lstrip()
        if stripped.startswith("#"):
            body = stripped[1:].strip()
            kind = body.split()[0] if body else ""
            if kind in ("ifdef", "ifndef"):
                rest = body[len(kind) :].strip()
                name = rest.split()[0] if rest else ""
                parent = emitting()
                if name in macros:
                    val = macros[name]
                    taking = parent and (val if kind == "ifdef" else not val)
                    stack.append((taking, parent))
                else:
                    # 未知: ブロックをそのまま残すため、taking は parent のまま
                    stack.append((parent, parent))
                    if emitting():
                        out.append(raw)
                i += 1
                continue
            if kind == "else":
                if stack:
                    taking, parent = stack.pop()
                    # else は「if 側を既知展開したときだけ」反転する
                    stack.append((parent and not taking, parent))
                i += 1
                continue
            if kind == "endif":
                if stack:
                    stack.pop()
                i += 1
                continue
        if emitting():
            out.append(raw)
        i += 1
    return "".join(out)


def skip_trivia(text: str, i: int) -> int:
    n = len(text)
    while i < n:
        c = text[i]
        if c in " \t\r\n":
            i += 1
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            i += 2
            while i < n and text[i] != "\n":
                i += 1
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            i += 2
            while i + 1 < n and not (text[i] == "*" and text[i + 1] == "/"):
                i += 1
            i = min(n, i + 2)
            continue
        break
    return i


def skip_string(text: str, i: int) -> int:
    n = len(text)
    quote = text[i]
    i += 1
    while i < n:
        if text[i] == "\\":
            i += 2
            continue
        if text[i] == quote:
            return i + 1
        i += 1
    return i


def match_keyword_at(text: str, i: int, word: str) -> bool:
    n = len(word)
    if text[i : i + n] != word:
        return False
    before = text[i - 1] if i > 0 else ""
    after = text[i + n] if i + n < len(text) else ""
    if before.isalnum() or before == "_":
        return False
    if after.isalnum() or after == "_":
        return False
    return True


def read_preprocessor(text: str, i: int) -> int:
    """# から始まる前処理指令（行継続付き）の終端を返す。"""
    n = len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            i = skip_string(text, i)
            continue
        if c == "/" and i + 1 < n and text[i + 1] in "/*":
            i = skip_trivia(text, i)
            continue
        if c == "\\":
            j = i + 1
            while j < n and text[j] in " \t":
                j += 1
            if j < n and text[j] == "\n":
                i = j + 1
                continue
            i += 1
            continue
        if c == "\n":
            return i + 1
        i += 1
    return i


def skip_angle(text: str, i: int) -> int:
    """text[i] == '<' の template 引数を読み飛ばす。"""
    n = len(text)
    depth = 0
    while i < n:
        c = text[i]
        if c in "\"'":
            i = skip_string(text, i)
            continue
        if c == "/" and i + 1 < n and text[i + 1] in "/*":
            i = skip_trivia(text, i)
            continue
        if c == "<":
            depth += 1
            i += 1
            continue
        if c == ">":
            depth -= 1
            i += 1
            if depth == 0:
                return i
            continue
        i += 1
    return i


def strip_leading_templates(masked: str) -> str:
    s = masked.lstrip()
    while s.startswith("template"):
        rest = s[len("template") :]
        k = 0
        while k < len(rest) and rest[k] in " \t\n\r":
            k += 1
        if k < len(rest) and rest[k] == "<":
            end = skip_angle(rest, k)
            s = rest[end:].lstrip()
        else:
            break
    return s


def parse_markers(text: str) -> tuple[set[str], set[str]]:
    defined: set[str] = set()
    used: set[str] = set()
    for line in text.splitlines():
        m = re.match(r"^\s*//\s*@exports\s+(.+)$", line)
        if m:
            defined.update(m.group(1).split())
            continue
        m = re.match(r"^\s*//\s*@dep\s+(\S+)", line)
        if m:
            token = m.group(1)
            used.add(token.split("::")[-1])
    return defined, used


def extract_defined(text: str) -> set[str]:
    # @exports は旧ブロック単位のヒントであり、名前空間先頭に付くと
    # 未使用アルゴリズムまで生き残るので定義名には使わない。
    extra: set[str] = set()
    masked = mask_comments_and_strings(text).strip()
    if not masked:
        return extra
    if masked.startswith("#"):
        m = re.match(r"#\s*define\s+([A-Za-z_]\w*)", masked)
        if m:
            extra.add(m.group(1))
        return extra
    s = strip_leading_templates(masked)
    if s.startswith("using"):
        rest = s[5:].lstrip()
        if rest.startswith("namespace"):
            return extra
        m = re.match(r"([A-Za-z_]\w*)", rest)
        if m:
            extra.add(m.group(1))
        return extra
    m = re.match(r"(?:struct|class|enum(?:\s+class)?)\s+([A-Za-z_]\w*)", s)
    if m:
        extra.add(m.group(1))
        return extra
    # 関数: 最初のトップレベル (
    angle = 0
    paren = 0
    i = 0
    while i < len(s):
        c = s[i]
        if c == "<":
            angle += 1
        elif c == ">":
            angle = max(0, angle - 1)
        elif c == "(" and angle == 0 and paren == 0:
            ident = _ident_before(s, i)
            if ident:
                extra.add(ident)
            qual = _qual_before(s, i)
            extra.update(qual)
            return extra
        elif c == "(":
            paren += 1
        elif c == ")":
            paren = max(0, paren - 1)
        elif c in "{;" and angle == 0 and paren == 0:
            break
        i += 1
    # 配列 / 変数
    m = re.search(
        r"([A-Za-z_]\w*)(?:\s*::\s*([A-Za-z_]\w*))?\s*(?:\[[^\]]*\]\s*)*(?:=|;)",
        s,
    )
    if m:
        extra.add(m.group(1))
        if m.group(2):
            extra.add(m.group(2))
    return extra


def _ident_before(s: str, i: int) -> str | None:
    j = i
    while j > 0 and s[j - 1] in " \t\n\r":
        j -= 1
    k = j
    while k > 0 and (s[k - 1].isalnum() or s[k - 1] == "_"):
        k -= 1
    ident = s[k:j]
    if ident and ident not in KEYWORDS:
        return ident
    return None


def _qual_before(s: str, i: int) -> set[str]:
    """Foo::bar( の Foo も定義名に含める（メンバ定義用）。"""
    out: set[str] = set()
    j = i
    while j > 0 and s[j - 1] in " \t\n\r":
        j -= 1
    # bar
    k = j
    while k > 0 and (s[k - 1].isalnum() or s[k - 1] == "_"):
        k -= 1
    rest = s[:k].rstrip()
    if rest.endswith("::"):
        rest = rest[:-2].rstrip()
        m = re.search(r"([A-Za-z_]\w*)$", rest)
        if m:
            out.add(m.group(1))
    return out


def extract_used(text: str) -> set[str]:
    _, marker_used = parse_markers(text)
    defined = extract_defined(text)
    used = extract_idents(text)
    used -= defined
    used |= marker_used
    return used


def _matching_brace_from(text: str, brace_idx: int) -> int:
    """text[brace_idx] == '{' に対応する '}' の位置（その直前 = inner end）。"""
    n = len(text)
    depth = 0
    i = brace_idx
    while i < n:
        c = text[i]
        if c in "\"'":
            i = skip_string(text, i)
            continue
        if c == "R" and i + 1 < n and text[i + 1] == '"':
            i = _skip_raw_string(text, i)
            continue
        if c == "/" and i + 1 < n and text[i + 1] in "/*":
            i = skip_trivia(text, i)
            continue
        if c == "{":
            depth += 1
            i += 1
            continue
        if c == "}":
            depth -= 1
            if depth == 0:
                return i
            i += 1
            continue
        i += 1
    return -1


def _skip_raw_string(text: str, i: int) -> int:
    j = i + 2
    n = len(text)
    while j < n and text[j] != "(":
        j += 1
    delim = text[i + 2 : j]
    end = f"){delim}\""
    k = text.find(end, j + 1)
    return n if k < 0 else k + len(end)


def split_units(text: str, ns: tuple[str, ...] = ()) -> list[Unit]:
    units: list[Unit] = []
    i = 0
    n = len(text)
    while i < n:
        if skip_trivia(text, i) >= n:
            if units:
                units[-1].text += text[i:]
            elif text[i:].strip():
                units.append(
                    Unit(
                        text=text[i:],
                        defined=set(),
                        used=set(),
                        ns=ns,
                        keep_always=True,
                    )
                )
            break
        start = i
        j = skip_trivia(text, i)
        if j >= n:
            break
        if text[j] == "#":
            k = read_preprocessor(text, j)
            chunk = text[start:k]
            units.append(_unit_from_chunk(chunk, ns))
            i = k
            continue
        ns_parts, ns_end = _try_split_namespace(text, start, j, ns)
        if ns_parts is not None:
            units.extend(ns_parts)
            i = ns_end
            continue
        k = scan_declaration_end(text, j)
        chunk = text[start:k]
        units.append(_unit_from_chunk(chunk, ns))
        i = k
    return units


def _try_split_namespace(
    text: str, start: int, j: int, ns: tuple[str, ...]
) -> tuple[list[Unit] | None, int]:
    if not match_keyword_at(text, j, "namespace"):
        return None, start
    # namespace から宣言末尾までを一塊として取り、内側を再帰分割
    k = scan_declaration_end(text, j)
    trivia = text[start:j]
    chunk = text[j:k]
    parsed = _parse_namespace_chunk(chunk)
    if parsed is None:
        u = _unit_from_chunk(text[start:k], ns)
        return [u], k
    name, _header, inner, _footer = parsed
    inner_units = split_units(inner, ns + (name,))
    if not inner_units:
        u = _unit_from_chunk(text[start:k], ns)
        return [u], k
    if trivia:
        inner_units[0].text = trivia + inner_units[0].text
    return inner_units, k


def _parse_namespace_chunk(chunk: str) -> tuple[str, str, str, str] | None:
    masked = mask_comments_and_strings(chunk)
    j = skip_trivia(masked, 0)
    if not match_keyword_at(masked, j, "namespace"):
        return None
    k = skip_trivia(masked, j + len("namespace"))
    m = IDENT_RE.match(masked, k)
    if not m:
        return None
    name = m.group(0)
    k = skip_trivia(masked, m.end())
    if k >= len(masked) or masked[k] != "{":
        return None
    inner_end = _matching_brace_from(chunk, k)
    if inner_end < 0:
        return None
    header = chunk[: k + 1]
    inner = chunk[k + 1 : inner_end]
    footer = chunk[inner_end:]
    return name, header, inner, footer


def scan_declaration_end(text: str, j: int) -> int:
    n = len(text)
    depth = 0
    angle = 0
    i = j
    saw_brace = False
    while i < n:
        c = text[i]
        if c in "\"'":
            i = skip_string(text, i)
            continue
        if c == "R" and i + 1 < n and text[i + 1] == '"':
            i = _skip_raw_string(text, i)
            continue
        if c == "/" and i + 1 < n and text[i + 1] in "/*":
            i = skip_trivia(text, i)
            continue
        if c == "<":
            # template 引数か比較か判別は粗いが、宣言頭部では template が多い
            angle += 1
            i += 1
            continue
        if c == ">":
            angle = max(0, angle - 1)
            i += 1
            continue
        if c == "{":
            depth += 1
            saw_brace = True
            i += 1
            continue
        if c == "}":
            depth -= 1
            i += 1
            if depth == 0 and saw_brace:
                while i < n and text[i] in " \t":
                    i += 1
                if i < n and text[i] == ";":
                    i += 1
                    while i < n and text[i] in " \t":
                        i += 1
                if i + 1 < n and text[i : i + 2] == "//":
                    while i < n and text[i] != "\n":
                        i += 1
                if i < n and text[i] == "\n":
                    i += 1
                return i
            continue
        if c == ";" and depth == 0:
            return i + 1
        i += 1
    return n


def _unit_from_chunk(chunk: str, ns: tuple[str, ...]) -> Unit:
    defined = extract_defined(chunk)
    used = extract_used(chunk)
    keep_always = False
    is_root = "main" in defined
    stripped = mask_comments_and_strings(chunk).strip()
    if stripped.startswith("#include"):
        keep_always = True
    if re.match(r"using\s+namespace\s+", stripped):
        keep_always = True
        is_root = False
    return Unit(
        text=chunk,
        defined=defined,
        used=used,
        ns=ns,
        keep_always=keep_always,
        is_root=is_root,
    )


def shake(units: list[Unit]) -> list[Unit]:
    live: set[str] = set()
    for u in units:
        if u.is_root:
            live |= u.used
            live |= u.defined
        elif u.keep_always:
            live |= u.used
    kept = [False] * len(units)
    changed = True
    while changed:
        changed = False
        for i, u in enumerate(units):
            if kept[i]:
                continue
            if u.keep_always or u.is_root or (u.defined & live):
                kept[i] = True
                live |= u.used
                live |= u.defined
                changed = True
    return [u for u, k in zip(units, kept) if k]


def strip_markers(text: str) -> str:
    lines = []
    for line in text.splitlines():
        if MARKER_RE.match(line):
            continue
        lines.append(line)
    return "\n".join(lines)


def _close_ns(parts: list[str], current: tuple[str, ...], target: tuple[str, ...]) -> tuple[str, ...]:
    while current and (len(current) > len(target) or current != target[: len(current)]):
        parts.append("\n} // namespace " + current[-1] + "\n")
        current = current[:-1]
    return current


def emit(units: list[Unit]) -> str:
    parts: list[str] = []
    current: tuple[str, ...] = ()
    for u in units:
        current = _close_ns(parts, current, u.ns)
        while len(current) < len(u.ns):
            nxt = u.ns[len(current)]
            parts.append(f"\nnamespace {nxt} {{\n")
            current += (nxt,)
        parts.append(u.text)
    _close_ns(parts, current, ())
    text = "".join(parts)
    text = strip_markers(text)
    text = re.sub(r"\n{3,}", "\n\n", text)
    if text and not text.endswith("\n"):
        text += "\n"
    return text.lstrip("\n")


def strip_source(source: str, macros: dict[str, bool] | None = None) -> str:
    expanded = expand_conditionals(source, macros or DEFAULT_MACROS)
    units = split_units(expanded)
    kept = shake(units)
    return emit(kept)


def is_complete_source(text: str) -> bool:
    return bool(
        re.search(r"^\s*#include\b", text, re.M)
        or re.search(r"\bint\s+main\s*\(", text)
    )


def inject_into_main(template: str, snippet: str) -> str:
    marker = "fastio();"
    idx = template.rfind(marker)
    snippet = snippet.strip("\n")
    if idx >= 0:
        insert_at = idx + len(marker)
        return template[:insert_at] + "\n" + snippet + "\n" + template[insert_at:]
    m = re.search(r"int\s+main\s*\(\)\s*\{", template)
    if m:
        return template[: m.end()] + "\n" + snippet + "\n" + template[m.end() :]
    return template + "\n" + snippet + "\n"


def load_source(path: Path | None, project_root: Path) -> str:
    template_path = project_root / "main.cpp"
    if path is None:
        if not template_path.exists():
            raise FileNotFoundError(f"{template_path} not found")
        return template_path.read_text()
    text = path.read_text()
    if is_complete_source(text):
        return text
    if not template_path.exists():
        raise FileNotFoundError(
            f"{path} は完全なソースではないため main.cpp が必要です"
        )
    return inject_into_main(template_path.read_text(), text)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="未使用宣言を削除して提出用コードを出力する"
    )
    parser.add_argument(
        "source",
        nargs="?",
        help="入力ファイル（省略時はプロジェクトの main.cpp）",
    )
    parser.add_argument(
        "-o",
        "--output",
        help="出力ファイル（省略時は stdout）",
    )
    parser.add_argument(
        "--stats",
        action="store_true",
        default=True,
        help="削減量を stderr に出す（デフォルト）",
    )
    parser.add_argument(
        "--no-stats",
        action="store_true",
        help="削減量を出さない",
    )
    args = parser.parse_args(argv)

    script_dir = Path(__file__).resolve().parent
    project_root = script_dir.parent
    src_path = Path(args.source).resolve() if args.source else None
    original = load_source(src_path, project_root)
    result = strip_source(original)

    show_stats = not args.no_stats
    if show_stats:
        before = len(original)
        after = len(result)
        pct = 0.0 if before == 0 else (1 - after / before) * 100
        print(
            f"stripped: {before} -> {after} bytes ({pct:.1f}% reduction)",
            file=sys.stderr,
        )

    if args.output:
        Path(args.output).write_text(result)
    else:
        sys.stdout.write(result)
    return 0


if __name__ == "__main__":
    sys.exit(main())
