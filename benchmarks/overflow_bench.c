/**
 * @file overflow_bench.c
 * @brief Overflow sorting algorithm using overflow bench.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000
#define THRESHOLD 65535
#define RUNS 100

void overflow_sort_scaled(uint16_t input[], int size, uint16_t sorted[]) {
  uint64_t scaled[SIZE];
  bool popped[SIZE];
  int pop_count;

  for (int i = 0; i < size; ++i) {
    scaled[i] = (uint64_t)input[i] * input[i];
    popped[i] = false;
  }

  pop_count = 0;
  while (pop_count < size) {
    for (int i = 0; i < size; ++i) {
      if (!popped[i]) {
        scaled[i] *= 2;
        if (scaled[i] > THRESHOLD) {
          popped[i] = true;
          int j = pop_count - 1;
          while (j >= 0 && input[i] > sorted[j]) {
            sorted[j + 1] = sorted[j];
            --j;
          }
          sorted[j + 1] = input[i];
          ++pop_count;
        }
      }
    }
  }
}

int main() {
  uint16_t input[SIZE];
  uint16_t sorted[SIZE];
  srand((unsigned int)time(NULL));

  clock_t start = clock();
  for (int run = 0; run < RUNS; ++run) {
    for (int i = 0; i < SIZE; ++i) {
      input[i] = rand() % 65536;
    }
    overflow_sort_scaled(input, SIZE, sorted);
  }
  clock_t end = clock();

  double total_time = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Total time for %d runs: %.6f seconds\\n", RUNS, total_time);
  printf("Average time per run: %.6f seconds\\n", total_time / RUNS);
  return 0;
}
