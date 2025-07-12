/**
 * @file overflow_sort_simd.c
 * @brief Optimized OverflowSort using AVX2 for uint16_t keys.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000
#define THRESHOLD 65535
#define MAX_ROUNDS 32

typedef struct {
  uint16_t value;
  int tick;
} OverflowEntry;

void overflow_sort_simd(uint16_t input[], int size) {
  OverflowEntry *result = _mm_malloc(size * sizeof(OverflowEntry), 32);
  int tick_counter = 0;

  // Compute max ticks
  __m256i max_val = _mm256_setzero_si256();
  for (int i = 0; i < size; i += 16) {
    __m256i vec = _mm256_loadu_si256((__m256i *)&input[i]);
    max_val = _mm256_max_epu16(max_val, vec);
  }

  uint16_t max_buf[16];
  _mm256_storeu_si256((__m256i *)max_buf, max_val);
  int max_ticks = 0;
  for (int i = 0; i < 16; i++) {
    int ticks = max_buf[i]
                    ? (16 - __builtin_clz(max_buf[i] * max_buf[i] / THRESHOLD))
                    : 0;
    max_ticks = ticks > max_ticks ? ticks : max_ticks;
  }
  max_ticks = max_ticks > 0 ? max_ticks : 1;

  // Allocate reusable buffers outside the loop
  uint16_t *vec_buf = (uint16_t *)_mm_malloc(16 * sizeof(uint16_t), 32);
  int16_t *tick_buf = (int16_t *)_mm_malloc(16 * sizeof(int16_t), 32);

  // Vectorized overflow loop
  for (int i = 0; i < size; i += 16) {
    int count = (size - i >= 16) ? 16 : size - i;
    __m256i vec = _mm256_loadu_si256((__m256i *)&input[i]);
    __m256i scaled_vec = vec;
    __m256i popped_vec = _mm256_setzero_si256();
    __m256i ticks_vec = _mm256_set1_epi16(-1);

    for (int r = 0; r < max_ticks; ++r) {
      scaled_vec = _mm256_add_epi16(scaled_vec, scaled_vec);
      __m256i overflow_mask = _mm256_cmpgt_epi16(_mm256_set1_epi16(THRESHOLD), scaled_vec);
      __m256i not_popped_mask = _mm256_cmpeq_epi16(popped_vec, _mm256_setzero_si256());
      __m256i valid_mask = _mm256_and_si256(overflow_mask, not_popped_mask);
      popped_vec = _mm256_or_si256(popped_vec, valid_mask);

      __m256i tick_vec = _mm256_set1_epi16(r);
      ticks_vec = _mm256_blendv_epi8(ticks_vec, tick_vec, valid_mask);

      if (_mm256_testz_si256(popped_vec, popped_vec) == 0)
        break;
    }

    _mm256_storeu_si256((__m256i *)vec_buf, vec);
    _mm256_storeu_si256((__m256i *)tick_buf, ticks_vec);

    for (int j = 0; j < count; j++) {
      result[i + j].value = vec_buf[j];
      result[i + j].tick = tick_buf[j];
    }
  }

  _mm_free(vec_buf);
  _mm_free(tick_buf);

  // Stable counting sort by tick
  int counts[MAX_ROUNDS + 1] = {0};
  for (int i = 0; i < size; i++) {
    int tick = result[i].tick;
    if (tick < 0 || tick > MAX_ROUNDS) tick = MAX_ROUNDS;
    counts[tick]++;
  }
  for (int i = 1; i <= MAX_ROUNDS; i++) {
    counts[i] += counts[i - 1];
  }

  uint16_t *temp = _mm_malloc(size * sizeof(uint16_t), 32);
  for (int i = size - 1; i >= 0; i--) {
    int tick = result[i].tick;
    if (tick < 0 || tick > MAX_ROUNDS) tick = MAX_ROUNDS;
    temp[--counts[tick]] = result[i].value;
  }

  //for (int i = 0; i < size; i += 16) {
  //  _mm256_storeu_si256((__m256i *)&input[i],
  //                      _mm256_loadu_si256((__m256i *)&temp[i]));
  //}

  // Final sorter
  for (int i = 0; i < size; i++) {
    input[i] = temp[i];
  }
  
  _mm_free(result);
  _mm_free(temp);
}


int main() {
  uint16_t input[SIZE];
  srand((unsigned int)time(NULL));
  for (int i = 0; i < SIZE; i++) {
    input[i] = rand() % 256;
  }
  clock_t start = clock();
  overflow_sort_simd(input, SIZE);
  clock_t end = clock();
  printf("Overflow sort with SIMD: %d elements\n", SIZE);
  printf("Elapsed time: %.6f seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);
  printf("First 16 sorted values:\n");
  for (int i = 0; i < 16; i++) {
    printf("%u\n", input[i]);
  }
  return 0;
}
