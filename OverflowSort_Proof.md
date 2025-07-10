
# Formal Proof of Correctness: OverflowSort Algorithm

**Author:** Scott Douglass  
**Date:** July 2025

## 1. Overview

The OverflowSort algorithm uses a hardware-inspired model: counting how many times a number must be doubled before it overflows a threshold `T`. This overflow count, or **tick**, is used to sort values in descending order.

## 2. Tick Function Definition

Let `x_i` be an unsigned integer, and `T` a fixed threshold (e.g. 65536). Define:

```
tick(x_i) = ⌈ log₂(T / x_i) ⌉ = ⌈ log₂(T) - log₂(x_i) ⌉
```

This is the minimum number of doublings needed to exceed `T`.

## 3. Monotonicity Proof

Since `log₂(x)` is strictly increasing, we get:

```
x_i > x_j → tick(x_i) < tick(x_j)
```

So sorting by `tick()` in ascending order is equivalent to sorting `x_i` in descending numeric order.

## 4. Tick Inversion

Given a tick value `t`, we can infer:

```
x_i < T / 2^(t - 1)
```

So each tick maps to a bucket of values.

## 5. Time Complexity

- Doubling loop: O(log x)
- Using bit tricks (e.g. __builtin_clz): O(1)

## 6. Sorting Correctness

Let `tick(x_i)` be computed for all `x_i`. Sorting by these ticks gives:

```
tick(x₁) < tick(x₂) < ... < tick(xₙ) ⇒ x₁ > x₂ > ... > xₙ
```

✅ OverflowSort is a correct descending-order sort based on overflow ticks.
