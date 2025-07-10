/**
 * @file overflow_sort_counting.c
 * @brief Overflow sorting algorithm using overflow sort counting.
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

#define SIZE 1000000
#define THRESHOLD 65535
#define MAX_ROUNDS 32

typedef struct {
  uint16_t value;
  int tick;
} OverflowEntry;

void overflow_sort_counting(uint16_t input[], int size,
                            OverflowEntry sorted[]) {
  uint64_t *scaled = malloc(sizeof(uint64_t) * size);
  bool *popped = calloc(size, sizeof(bool));
  OverflowEntry *temp = malloc(sizeof(OverflowEntry) * size);
  int *count = calloc(size, sizeof(int));
  int tick_counter = 0;

  for (int i = 0; i < size; ++i) {
    scaled[i] = (uint64_t)input[i] * input[i];
    temp[i].value = input[i];
    temp[i].tick = -1;
  }

  for (int round = 0; round < MAX_ROUNDS; ++round) {
    int popped_this_round = 0;
    for (int i = 0; i < size; ++i) {
      if (!popped[i]) {
        scaled[i] *= 2;
        if (scaled[i] > THRESHOLD) {
          popped[i] = true;
          temp[i].tick = tick_counter++;
          popped_this_round++;
        }
      }
    }
    if (popped_this_round == 0)
      break;
  }

  for (int i = 0; i < size; ++i) {
    if (temp[i].tick >= 0)
      count[temp[i].tick]++;
  }

  for (int i = 1; i < size; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = size - 1; i >= 0; --i) {
    if (temp[i].tick >= 0) {
      int pos = --count[temp[i].tick];
      sorted[pos] = temp[i];
    }
  }

  free(scaled);
  free(popped);
  free(temp);
  free(count);
}

int main() {
  uint16_t *input = malloc(sizeof(uint16_t) * SIZE);
  OverflowEntry *sorted = malloc(sizeof(OverflowEntry) * SIZE);
  srand((unsigned int)time(NULL));

  for (int i = 0; i < SIZE; ++i) {
    input[i] = rand() % 256;
  }

  clock_t start = clock();
  overflow_sort_counting(input, SIZE, sorted);
  clock_t end = clock();

  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Overflow sort with counting sort: %d elements\n", SIZE);
  printf("Elapsed time: %.6f seconds\n", elapsed);

  free(input);
  free(sorted);
  return 0;
}
