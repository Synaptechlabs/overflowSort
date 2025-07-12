/**
 * @file overflow_sort_avx2.c
 * @brief Optimized OverflowSort using AVX2 for uint32_t keys.
 *
 * @author Scott Douglass
 * @date 2025-07-11
 * @license MIT
 */

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10000
#define LANES 8
#define MAX_TICKS                                                              \
  32 // Reduced from 128, as most uint32_t overflow in <=32 ticks

// Slimmed-down struct: no index, stability via counting sort
typedef struct {
  uint32_t value;
  int ticks;
} TickResult;

void overflow_sort_avx2(uint32_t *input, int size, uint32_t *output) {
  TickResult *results =
      _mm_malloc(size * sizeof(TickResult), 32); // Aligned for SIMD
  int processed = 0;

  while (processed < size) {
    int count = (size - processed >= LANES) ? LANES : size - processed;

    // Load chunk
    __m256i vec = _mm256_loadu_si256((__m256i *)&input[processed]);
    __m256i prev = vec;
    __m256i ticks = _mm256_setzero_si256();
    __m256i popped = _mm256_setzero_si256();

    // Compute max ticks needed with _mm256_max_epu32
    uint32_t max_buf[LANES];
    _mm256_storeu_si256((__m256i *)max_buf, vec);
    int max_ticks = 0;
    for (int i = 0; i < count; i++) {
      int ticks_needed = max_buf[i] ? (32 - __builtin_clz(max_buf[i])) : 0;
      max_ticks = ticks_needed > max_ticks ? ticks_needed : max_ticks;
    }
    max_ticks = max_ticks > 0 ? max_ticks : 1; // Ensure at least 1 tick

    // Pre-shift small values to reduce ticks
    __m256i shift_mask = _mm256_cmpgt_epi32(_mm256_set1_epi32(0xFFFF), vec);
    vec = _mm256_blendv_epi8(vec, _mm256_slli_epi32(vec, 8), shift_mask);

    // Main loop with dynamic ticks
    for (int t = 1; t <= max_ticks; ++t) {
      prev = vec;
      vec = _mm256_add_epi32(vec, vec); // Double values

      __m256i overflow_mask = _mm256_cmpgt_epi32(prev, vec);
      __m256i not_popped_mask =
          _mm256_cmpeq_epi32(popped, _mm256_setzero_si256());
      __m256i valid_mask = _mm256_and_si256(overflow_mask, not_popped_mask);

      __m256i tick_val = _mm256_set1_epi32(t);
      popped = _mm256_or_si256(popped, valid_mask);
      ticks = _mm256_blendv_epi8(ticks, tick_val, valid_mask);

      // SIMD pop check
      if (_mm256_testz_si256(popped, popped) == 0)
        break;
    }

    // Vectorized write to results
    _mm256_storeu_si256((__m256i *)&results[processed].value,
                        _mm256_loadu_si256((__m256i *)&input[processed]));
    _mm256_storeu_si256((__m256i *)&results[processed].ticks, ticks);

    processed += count;
  }

  // Counting sort by ticks (stable)
  int counts[MAX_TICKS + 1] = {0};
  for (int i = 0; i < size; i++)
    counts[results[i].ticks]++;
  for (int i = 1; i <= MAX_TICKS; i++)
    counts[i] += counts[i - 1];
  uint32_t *temp = _mm_malloc(size * sizeof(uint32_t), 32);
  for (int i = size - 1; i >= 0; i--) {
    temp[--counts[results[i].ticks]] = results[i].value;
  }

  // Vectorized copy to output
  for (int i = 0; i < size; i += LANES) {
    _mm256_storeu_si256((__m256i *)&output[i],
                        _mm256_loadu_si256((__m256i *)&temp[i]));
  }

  _mm_free(results);
  _mm_free(temp);
}

int main() {
  uint32_t input[SIZE];
  uint32_t output[SIZE];

  srand((unsigned int)time(NULL));
  for (int i = 0; i < SIZE; i++) {
    input[i] = rand() % 0x7FFFFFFF;
  }

  overflow_sort_avx2(input, SIZE, output);

  printf("First 16 sorted values by AVX2 overflow ticks:\n");
  for (int i = 0; i < 16; i++) {
    printf("%u\n", output[i]);
  }

  return 0;
}
