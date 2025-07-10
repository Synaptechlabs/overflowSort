# ⚡ overflowSort — Overflow-Based Integer Sorting

[![GitHub Pages](https://img.shields.io/badge/view--site-overflowSort-brightgreen)](https://synaptechlabs.github.io/overflowSort)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

**Author:** Scott Douglass  
**License:** MIT  
**Version:** 1.0.0  
**Date:** 2025-07-10  

---

## 🔍 What Is overflowSort?

**overflowSort** is a set of custom sorting algorithms that use simulated or real **integer overflow behavior** to control sort order. Instead of relying on standard comparison logic, it uses multiplication until values exceed hardware thresholds — effectively "popping" elements into order.

Variants are written in C, with SIMD/AVX2 acceleration and real benchmark comparisons to `qsort` and radix sort.

---

## 🧠 Key Features

- Overflow-based sort logic using integer multiplication
- Scaled overflow triggers for tuning
- Counting-sort hybrid version
- SIMD and AVX2 accelerated variants
- Integer and string sorting capability
- Real-world benchmark suite
- Clean C99 source and portable

---

## 🗂️ File Structure

```
src/
├── overflow_sort_scaled.c         # Scaled overflow variant
├── overflow_sort_simd.c          # SIMD-based version
├── overflow_sort_counting.c      # Overflow + counting sort hybrid
├── overflow_sort_avx2.c          # AVX2-accelerated variant

benchmarks/
├── overflow_vs_radix_vs_qsort.c  # Head-to-head timing
├── sort_scaling_benchmark.c      # Scaling tests
├── overflow_vs_qsort_avx2.c      # SIMD vs standard comparison

tests/
├── uint8_t.c                      # Mini testbed for 8-bit overflow logic
```

---

## 🛠️ Build & Run Instructions

Each file is standalone and can be compiled with `gcc`. Below are common builds:

### 1. Basic Overflow Sort (Scaled):
```bash
gcc -O2 -o overflow_sort src/overflow_sort_scaled.c
./overflow_sort
```

### 2. Counting Sort Hybrid:
```bash
gcc -O2 -o overflow_count src/overflow_sort_counting.c
./overflow_count
```

### 3. AVX2 Version (Requires AVX2 CPU):
```bash
gcc -O2 -mavx2 -o overflow_avx2 src/overflow_sort_avx2.c
./overflow_avx2
```

### 4. Full Benchmark Comparison:
```bash
gcc -O2 -lm -o compare benchmarks/overflow_vs_radix_vs_qsort.c
./compare
```

---

## 📊 Benchmarks (Real-World, 10 Million Integers)

| Algorithm        | Time (Seconds) |
|------------------|----------------|
| Overflow Sort    | 0.685          |
| qsort (glibc)    | 1.082          |
| Radix Sort (C)   | 0.057          |

Radix sort is fastest asymptotically, but overflowSort shows compelling performance given its simplicity and novel mechanics — especially in SIMD or constrained environments.

---

## 🌐 Project Website

→ Live demo site:  
[https://synaptechlabs.github.io/overflowSort](https://synaptechlabs.github.io/overflowSort)

---

## 📣 Contact & Credit

Made by **Scott Douglass**  
[@SynaptechLabs](https://twitter.com/SynaptechLabs)

---

## 🧪 Notes & Ideas

- Sorting by overflow "ticks" opens up hardware acceleration possibilities
- Can be extended to strings by interpreting ASCII overflow
- Visualization of overflow behavior shows stability under scaling

---

## 📄 License (MIT)

This project is licensed under the MIT License:

> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files...  
> *(See LICENSE file for full terms.)*

---
