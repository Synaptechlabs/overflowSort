# Pop Sort: Overflow-Based Sorting Algorithms

**Author:** Scott Douglass  
**License:** MIT  
**Version:** 1.0.0  
**Date:** 2025-07-10

## Overview

Pop Sort is a novel class of sorting algorithms that leverages simulated or hardware-inspired overflow behavior to prioritize and order data. This repository contains multiple variants of the algorithm, benchmarks against standard methods (e.g., `qsort`, radix sort), and SIMD-accelerated versions.

## Key Features

- Overflow-based sorting logic
- SIMD acceleration (SSE/AVX2)
- Counting and scaled strategies
- Benchmarked vs `qsort` and radix sort
- Lightweight and C99-compatible

## Structure

```
src/            # Core sorting algorithm implementations
benchmarks/     # Performance comparisons and tests
docs/           # Benchmark results, methodology, build instructions
build/          # Optional build scripts
include/        # Reserved for future headers
```

## Build Instructions

See [`BUILD.md`](docs/BUILD.md)

## Benchmark Results

See [`BENCHMARKS.md`](docs/BENCHMARKS.md)
