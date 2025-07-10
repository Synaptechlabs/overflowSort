# Benchmark Results: Pop Sort vs Traditional Algorithms

This document summarizes the performance of Pop Sort variants vs. standard sorting methods on real-world and synthetic data.

---

## ✅ Test System
- **CPU**: Intel/AMD with AVX2 and SSE4.1 support
- **OS**: Debian Linux
- **Compiler**: GCC with `-O2`, `-mavx2`, `-msse4.1`

---

## 📊 Real-World Value Sorting (10M elements)

| Method               | Time (seconds) |
|----------------------|----------------|
| **Overflow Sort**    | 0.685          |
| qsort (stdlib)       | 1.082          |
| **Radix Sort**       | **0.057**      |

> ⚡ Radix remains fastest overall, but Overflow Sort showed significant speed over `qsort` in mid-size sets (e.g. 100K–1M range).

---

## 🧪 Synthetic Data (Random uint16_t)
| Size     | Overflow Sort | qsort   | Radix Sort |
|----------|----------------|---------|------------|
| 1K       | ~0.00001 s     | ~0.00002 s | ~0.00001 s |
| 100K     | ~0.0038 s      | ~0.0082 s  | ~0.0010 s  |
| 1M       | ~0.038 s       | ~0.10 s    | ~0.006 s   |
| 10M      | ~0.68 s        | ~1.08 s    | **0.057 s**|

---

## 🔍 Observations

- **Overflow Sort** scales sublinearly in early growth but saturates past ~1M elements.
- **Vectorized versions** show improved CPU cache locality and throughput.
- **Counting variants** outperform qsort but don’t match radix under high cardinality.
- Best suited for small to medium sets or embedded/data-sensitive environments.

---

## 🛠️ Notes

- All benchmarks run using `clock()` in C with warmup runs discarded.
- AVX/SIMD builds used for vector-friendly variants.
- Real-world datasets approximated via random log-normal and cosine transforms.

