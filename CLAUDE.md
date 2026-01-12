# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a competitive programming template for C++23. The main.cpp file contains commonly used type aliases, macros, constants, and algorithm implementations for AtCoder and similar competitive programming contests.

## Build and Test Commands

Build the project (outputs main.out):

```bash
make build
```

Run tests using online-judge-tools (oj):

```bash
make test
```

The test command runs `oj t -c "./main.out" -d ./tests`. Test cases should be placed in the `./tests` directory.

## Compiler Configuration

The project uses g++ with C++23 standard (`-std=c++23`) and includes `-Wall -g` flags for warnings and debugging.

## Template Structure

main.cpp provides:

- Type aliases (ll, ld, pii, vi, vl, etc.)
- Constants (INF, LINF, MOD, MOD998, PI, EPS)
- Loop macros (rep, rep1, rrep, fore)
- Container helpers (all, rall, pb, eb, mp, fi, se)
- `fastio()` for I/O optimization
- `math` namespace with pow_mod, is_prime, combination, gcd, lcm
- `structure` namespace with UnionFind and SegTree
- `binary_search` namespace with integer and real binary search
- `graph` namespace with Dijkstra's algorithm
- RollingHash for string matching
