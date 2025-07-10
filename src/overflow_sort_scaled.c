/**
 * @file overflow_sort_scaled.c
 * @brief Overflow sorting algorithm using overflow sort scaled.
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

#define SIZE 500
#define THRESHOLD 65535   // Simulated 16-bit overflow threshold
#define SCALE_FACTOR 1011 // Spread values to reduce collisions

void overflow_sort_scaled(uint16_t input[], int size, uint16_t sorted[]) {
  uint64_t scaled[SIZE];
  bool popped[SIZE] = {false};
  int pop_count = 0;
  int scale = 1;

  for (int i = 0; i < size; ++i) {
    scaled[i] = input[i] * input[i];
  }

  while (pop_count < size) {
    for (int i = 0; i < size; ++i) {
      if (!popped[i]) {
        scaled[i] *= 2;
        if (scaled[i] > THRESHOLD) {
          popped[i] = true;

          // Insert in correct position (reverse order)
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

  srand((unsigned int)time(NULL)); // Seed RNG

  for (size_t i = 0; i < SIZE; ++i) {
    input[i] = rand() % 65536; // Random uint16_t value (0–65535)
  }

  overflow_sort_scaled(input, SIZE, sorted);

  printf("Sorted array (reverse order of overflow): ");
  for (int i = 0; i < SIZE; ++i) {
    printf("%d \n", sorted[i]);
  }
  printf("\n");

  return 0;
}
