/**
 * @file SIMD-Multiply-Sort.c
 * @brief Overflow sorting algorithm using simd-multiply-sort.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */


#include <immintrin.h> // For SSE2 intrinsics
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000 // Number of elements in the array
#define ROUNDS 16    // Number of repeated multiplications

void multiply_u16_simd(uint16_t *data, size_t size, int rounds) {
  size_t i;
  for (int r = 0; r < rounds; ++r) {
    for (i = 0; i + 8 <= size; i += 8) {
      __m128i vec = _mm_loadu_si128((__m128i *)&data[i]);
      vec = _mm_mullo_epi16(vec, vec); // Multiply each u16 by itself
      _mm_storeu_si128((__m128i *)&data[i], vec);
    }
    // Fallback for leftover elements
    for (; i < size; ++i) {
      data[i] *= data[i];
    }
  }
}

int main() {
  uint16_t data[SIZE];
  srand((unsigned int)time(NULL));

  // Initialize input with small values to avoid overflow too early
  for (size_t i = 0; i < SIZE; ++i) {
    data[i] = rand() % 256; // Values 0–255
  }

  clock_t start = clock();
  multiply_u16_simd(data, SIZE, ROUNDS);
  clock_t end = clock();

  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  printf("SIMD multiply: %zu elements × %d rounds\n", SIZE, ROUNDS);
  printf("Elapsed time: %.6f seconds\n", elapsed);

  return 0;
}
