# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a competitive programming template for C++23. The main.cpp file contains commonly used type aliases, macros, constants, and algorithm implementations for AtCoder and similar competitive programming contests.

## Build Commands

Build the project (outputs main.out):

```bash
ninja
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
- `math` namespace with pow_mod, is_prime, sieve, prime_list, factorize, divisors, extgcd, combination
- `structure` namespace with BIT, UnionFind, WeightedUnionFind, SegTree, LazySegTree
- `binary_search` namespace with integer and real binary search
- Compress (coordinate compression)
- CumulativeSum (1D and 2D prefix sums)
- RollingHash for string matching
- `graph` namespace with Dijkstra, BFS, Warshall-Floyd, topological sort, Kruskal (MST), LCA
