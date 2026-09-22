#!/usr/bin/env python3
"""提出用ストリッパーの単体・結合テスト。"""

from __future__ import annotations

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import strip  # noqa: E402


SAMPLE = r"""
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
constexpr int INF = 1e9;
constexpr ll LINF = 1e18;
#define all(v) (v).begin(), (v).end()
#define Sort(v) sort(all(v))
#define unused_macro(v) (v)
template <typename T> bool chmin(T &a, const T &b) {
  if (a > b) {
    a = b;
    return true;
  }
  return false;
}
template <typename T> bool chmax(T &a, const T &b) {
  if (a < b) {
    a = b;
    return true;
  }
  return false;
}
namespace math {
ll add1(ll x) { return x + 1; }
ll unused_fn(ll x) { return x; }
} // namespace math
namespace graph {
vector<ll> dijkstra(int s) {
  vector<ll> dist(1, LINF);
  dist[s] = 0;
  return dist;
}
int unused_graph() { return 0; }
} // namespace graph
#ifdef LOCAL
#define debug(...) 1
#else
#define debug(...) (void)0
#endif
#ifndef TESTING
int main() {
  fastio();
  ll x = math::add1(1);
  auto d = graph::dijkstra(0);
  Sort(d);
  return 0;
}
#endif
void fastio() {}
"""


class ExpandConditionalsTest(unittest.TestCase):
    def test_drops_local_debug_impl(self):
        src = "#ifdef LOCAL\nint keep_if_local;\n#else\nint keep_if_not;\n#endif\n"
        out = strip.expand_conditionals(src, {"LOCAL": False, "TESTING": False})
        self.assertIn("keep_if_not", out)
        self.assertNotIn("keep_if_local", out)
        self.assertNotIn("#ifdef", out)

    def test_unwraps_ifndef_testing(self):
        src = "#ifndef TESTING\nint main() { return 0; }\n#endif\n"
        out = strip.expand_conditionals(src, {"LOCAL": False, "TESTING": False})
        self.assertIn("int main()", out)
        self.assertNotIn("#ifndef", out)
        self.assertNotIn("#endif", out)


class TreeShakeTest(unittest.TestCase):
    def _shake(self, src: str) -> str:
        return strip.strip_source(src)

    def test_keeps_used_functions_and_aliases(self):
        out = self._shake(SAMPLE)
        self.assertIn("using ll = long long;", out)
        self.assertIn("add1", out)
        self.assertIn("dijkstra", out)
        self.assertIn("LINF", out)
        self.assertIn("#define all", out)
        self.assertIn("#define Sort", out)
        self.assertIn("void fastio()", out)
        self.assertIn("int main()", out)

    def test_drops_unused_declarations(self):
        out = self._shake(SAMPLE)
        self.assertNotIn("unused_fn", out)
        self.assertNotIn("unused_graph", out)
        self.assertNotIn("unused_macro", out)
        self.assertNotIn("using pii", out)
        self.assertNotIn("constexpr int INF", out)
        self.assertNotIn("chmin", out)
        self.assertNotIn("chmax", out)
        self.assertNotIn("#define endl", out)
        self.assertIn("} // namespace math", out)
        self.assertIn("} // namespace graph", out)

    def test_strips_markers(self):
        src = SAMPLE.replace(
            "namespace math {",
            "// @begin math\n// @exports add1\nnamespace math {",
        ).replace("} // namespace math", "} // namespace math\n// @end math")
        out = self._shake(src)
        self.assertNotIn("@begin", out)
        self.assertNotIn("@exports", out)
        self.assertNotIn("@end", out)

    def test_kruskal_keeps_unionfind_via_body_idents(self):
        src = r"""
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
namespace structure {
struct UnionFind {
  vector<int> d;
  UnionFind(int n = 0) : d(n, -1) {}
  int find(int x) { return d[x] < 0 ? x : d[x] = find(d[x]); }
  bool unite(int x, int y) { return find(x) != find(y); }
};
struct BIT {
  int n;
  BIT(int n_) : n(n_) {}
};
} // namespace structure
namespace graph {
ll kruskal(int n) {
  structure::UnionFind uf(n);
  return 0;
}
} // namespace graph
int main() {
  ll x = graph::kruskal(3);
  return 0;
}
"""
        out = self._shake(src)
        self.assertIn("UnionFind", out)
        self.assertIn("kruskal", out)
        self.assertNotIn("struct BIT", out)


class SnippetAndCliTest(unittest.TestCase):
    def test_snippet_is_injected_into_template_main(self):
        template = r"""
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll dummy_unused() { return 0; }
ll used() { return 1; }
int main() {
  fastio();

  return 0;
}
void fastio() {}
"""
        snippet = "ll ans = used();\n"
        combined = strip.inject_into_main(template, snippet)
        out = strip.strip_source(combined)
        self.assertIn("used()", out)
        self.assertIn("ll ans = used();", out)
        self.assertNotIn("dummy_unused", out)

    def test_cli_writes_output_file(self):
        with tempfile.TemporaryDirectory() as td:
            src_path = Path(td) / "solution.cpp"
            out_path = Path(td) / "submission.cpp"
            src_path.write_text(
                "#include <bits/stdc++.h>\nusing namespace std;\nint main(){return 0;}\n"
            )
            proc = subprocess.run(
                [
                    sys.executable,
                    str(ROOT / "tools" / "strip.py"),
                    str(src_path),
                    "-o",
                    str(out_path),
                ],
                check=True,
                capture_output=True,
                text=True,
            )
            self.assertTrue(out_path.exists())
            text = out_path.read_text()
            self.assertIn("int main()", text)
            self.assertIn("stripped:", proc.stderr)


class IntegrationMainCppTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.main_cpp = (ROOT / "main.cpp").read_text()

    def test_empty_main_drops_heavy_algorithms(self):
        out = strip.strip_source(self.main_cpp)
        for token in (
            "dijkstra",
            "MaxFlow",
            "LazySegTree",
            "suffix_array",
            "modint",
            "warshall_floyd",
            "combination",
        ):
            self.assertNotIn(token, out, f"{token} should be stripped from empty main")
        self.assertIn("fastio", out)
        self.assertIn("int main()", out)
        self.assertLess(len(out), len(self.main_cpp) // 4)

    def test_dijkstra_solution_keeps_graph_but_not_flow(self):
        snippet = r"""
  INT(n, m, s);
  vector<vector<pair<int, ll>>> g(n);
  rep(i, m) {
    INT(u, v);
    LL(c);
    g[u].eb(v, c);
  }
  auto dist = graph::dijkstra(g, s);
  print_vec(dist);
"""
        combined = strip.inject_into_main(self.main_cpp, snippet)
        out = strip.strip_source(combined)
        self.assertIn("dijkstra", out)
        self.assertIn("using ll", out)
        self.assertIn("#define rep", out)
        self.assertIn("#define INT", out)
        self.assertIn("print_vec", out)
        self.assertNotIn("struct MaxFlow", out)
        self.assertNotIn("bellman_ford(", out)
        self.assertNotIn("suffix_array(", out)
        self.assertIn("#define INT", out)
        self.assertIn("input_from_cin", out)
        self.assertIn("int __VA_ARGS__", out)

    def test_stripped_dijkstra_solution_compiles(self):
        snippet = r"""
  INT(n, m, s);
  vector<vector<pair<int, ll>>> g(n);
  rep(i, m) {
    INT(u, v);
    LL(c);
    g[u].eb(v, c);
  }
  auto dist = graph::dijkstra(g, s);
  print_vec(dist);
"""
        combined = strip.inject_into_main(self.main_cpp, snippet)
        out = strip.strip_source(combined)
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / "submission.cpp"
            bin_path = Path(td) / "submission.out"
            src.write_text(out)
            subprocess.run(
                ["g++", "-std=c++23", "-O2", "-o", str(bin_path), str(src)],
                check=True,
                capture_output=True,
                text=True,
            )
            self.assertTrue(bin_path.exists())

    def test_stripped_empty_main_compiles(self):
        out = strip.strip_source(self.main_cpp)
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / "submission.cpp"
            bin_path = Path(td) / "submission.out"
            src.write_text(out)
            subprocess.run(
                ["g++", "-std=c++23", "-O2", "-o", str(bin_path), str(src)],
                check=True,
                capture_output=True,
                text=True,
            )
            self.assertTrue(bin_path.exists())


if __name__ == "__main__":
    unittest.main()
