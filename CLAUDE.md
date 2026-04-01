# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a competitive programming template for C++23. The main.cpp file contains commonly used type aliases, macros, constants, and algorithm implementations for AtCoder and similar competitive programming contests.

## Build Commands

Build the project (outputs main.out):

```bash
ninja
```

Build and run tests:

```bash
ninja tests
for t in tests/*.out; do ./$t; done
```

## Compiler Configuration

The project uses g++ with C++23 standard (`-std=c++23`) and includes `-Wall -g` flags for warnings and debugging.

## Template Structure

main.cpp provides:

- Type aliases (ll, ld, pii, vi, vl, etc.)
- Constants (INF, LINF, MOD, MOD998, PI, EPS)
- Grid direction arrays (dx4, dy4, dx8, dy8)
- Loop macros (rep, rep1, rrep, fore)
- Container helpers (all, rall, pb, eb, mp, fi, se)
- Debug macro (`debug(...)` - enabled with `-DLOCAL`, supports pair/vector/set/map)
- `chmin`/`chmax` helpers
- `fastio()` for I/O optimization
- Input/output helpers (`input<T>()`, `input_vec<T>(n)`, `input_vec2<T>(n,m)`, `print_vec`)
- `modint` struct with `mint` (MOD=1e9+7) and `mint998` (MOD=998244353)
- `math` namespace with pow_mod, is_prime, sieve, prime_list, factorize, divisors, extgcd, combination, crt, floor_sum, euler_totient, euler_totient_table
- `structure` namespace with BIT, UnionFind, WeightedUnionFind, SegTree, LazySegTree, SparseTable
- `binary_search` namespace with integer and real binary search
- Compress (coordinate compression)
- CumulativeSum (1D and 2D prefix sums)
- Matrix (matrix multiplication and exponentiation)
- RollingHash for string matching
- `string_algo` namespace with z_algorithm, kmp_table, kmp_search, suffix_array, lcp_array
- `graph` namespace with Dijkstra, BFS, Warshall-Floyd, topological sort, Kruskal (MST), LCA, Bellman-Ford, SCC, MaxFlow (Dinic), EulerTour

## Tests

Tests are in `tests/` directory with one file per feature area. Each test file includes `main.cpp` with `-DTESTING` flag (which guards the template's `main()` function). Test framework is a minimal assertion-based header (`tests/test_helper.hpp`).

## Submission Code Stripper

`tools/strip.py` generates minimal submission code by removing unused template blocks:

```bash
python3 tools/strip.py solution.cpp > submission.cpp
```

Blocks in main.cpp are marked with `// @begin name` / `// @end name` comments. Dependencies are declared with `// @dep name` and exported identifiers with `// @exports id1 id2 ...`.
