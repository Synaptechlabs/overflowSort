/**
 * @file uint8_t.c
 * @brief Test file to explore overflow behavior of `uint8_t` values.
 *
 * This standalone test demonstrates how unsigned 8-bit integers behave when
 * subjected to repeated arithmetic operations, particularly multiplication
 * and overflow detection. It was originally used to validate the core principle
 * behind the Pop Sort algorithm.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define N 5

int main() {
  uint8_t a[N] = {2, 8, 3, 5, 9};
  bool overflowed[N] = {false};
  int sorted[N];
  int count = 0;
  int multiplier = 1;

  while (count < N) {
    for (int i = 0; i < N; i++) {
      if (!overflowed[i]) {
        uint16_t result = a[i] * multiplier;
        if (result > 255) {
          overflowed[i] = true;
          sorted[count++] = a[i];
        }
      }
    }
    multiplier *= 2; // Bit-shift would also work: multiplier <<= 1;
  }

  // Output sorted result (reverse order of overflow)
  printf("Sorted array (reverse order of overflow): ");
  for (int i = 0; i < N; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");

  return 0;
}
