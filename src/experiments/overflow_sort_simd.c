/**
 * @file overflow_sort_simd.c
 * @brief Overflow sorting algorithm using overflow sort simd.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */

#include <immintrin.h>
#include <stdbool.h>
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

int compare_ticks(const void *a, const void *b) {
  return ((OverflowEntry *)a)->tick - ((OverflowEntry *)b)->tick;
}

void overflow_sort_simd(uint16_t input[], int size, OverflowEntry result[]) {
  uint64_t scaled[SIZE];
  bool popped[SIZE] = {false};
  int tick_counter = 0;

  for (int i = 0; i < size; ++i) {
    scaled[i] = (uint64_t)input[i] * input[i];
    result[i].value = input[i];
    result[i].tick = -1;
  }

  for (int round = 0; round < MAX_ROUNDS; ++round) {
    int popped_this_round = 0;

    for (int i = 0; i + 8 <= size; i += 8) {
      for (int j = 0; j < 8; ++j) {
        int idx = i + j;
        if (!popped[idx]) {
          scaled[idx] *= 2;
          if (scaled[idx] > THRESHOLD) {
            popped[idx] = true;
            result[idx].tick = tick_counter++;
            popped_this_round++;
          }
        }
      }
    }

    for (int i = (size & ~7); i < size; ++i) {
      if (!popped[i]) {
        scaled[i] *= 2;
        if (scaled[i] > THRESHOLD) {
          popped[i] = true;
          result[i].tick = tick_counter++;
          popped_this_round++;
        }
      }
    }

    if (popped_this_round == 0)
      break;
  }

  qsort(result, size, sizeof(OverflowEntry), compare_ticks);
}

int main() {
  uint16_t input[SIZE];
  OverflowEntry result[SIZE];
  srand((unsigned int)time(NULL));

  for (int i = 0; i < SIZE; ++i) {
    input[i] = rand() % 256;
  }

  clock_t start = clock();
  overflow_sort_simd(input, SIZE, result);
  clock_t end = clock();

  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Overflow sort with SIMD assist: %d elements\n", SIZE);
  printf("Elapsed time: %.6f seconds\n", elapsed);

  return 0;
}
