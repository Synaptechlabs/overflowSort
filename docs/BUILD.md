# Build Instructions

## Prerequisites

- GCC (with AVX2 support for AVX-based files)
- Make (optional, for convenience)
- Linux or Unix-like environment recommended
- C99 or later

## Compilation Examples

### General Overflow Sort (Scaled)
```bash
gcc -O2 src/overflow_sort_scaled.c -o build/overflow_sort_scaled
```

### Counting Sort Variant
```bash
gcc -O2 src/overflow_sort_counting.c -o build/overflow_sort_counting
```

### AVX2 Optimized
```bash
gcc -O2 -mavx2 src/overflow_sort_avx2.c -o build/overflow_sort_avx2
```

### SIMD Variant
```bash
gcc -O2 -msse4.1 src/overflow_sort_simd.c -o build/overflow_sort_simd
```

### Benchmark vs qsort and radix
```bash
gcc -O2 benchmarks/overflow_vs_radix_vs_qsort.c -o build/overflow_vs_radix_vs_qsort -lm
```

## Using Makefile

To build everything:
```bash
make
```

To clean:
```bash
make clean
```
