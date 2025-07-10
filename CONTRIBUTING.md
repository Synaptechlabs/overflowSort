# Contributing to overflowSort

Thank you for considering contributing to **overflowSort** — a novel class of sorting algorithms leveraging overflow behavior for high-performance data sorting.

---

## 🚀 Getting Started

1. **Fork the repo**
2. Clone your fork locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/overflowSort.git
   cd overflowSort
   ```

3. Create a new branch:
   ```bash
   git checkout -b my-feature-branch
   ```

4. Make your changes, then:
   ```bash
   git add .
   git commit -m "Add feature or fix"
   git push origin my-feature-branch
   ```

5. Submit a Pull Request to `main` with a clear description.

---

## 🧠 Ideas for Contribution

- 🔍 Optimize the existing overflow sort implementation
- 🧮 Add float or string sorting variants
- 🚀 Benchmark against new algorithms or hardware
- 🧪 Add unit tests or stress tests
- 🛠 Improve SIMD or AVX2 usage
- 📦 Create a library version or port to other languages

---

## 🧪 Running Benchmarks

To test performance:

```bash
gcc -O2 benchmarks/overflow_vs_radix_vs_qsort.c -o sort_benchmark -lm
./sort_benchmark
```

Make sure to test on real-world and randomized data.

---

## 📃 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Maintained by:**  
Scott Douglass (@SynaptechLabs)  
https://synaptechlabs.ai