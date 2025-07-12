#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define THRESHOLD 255

uint16_t sorted_array[SIZE];
int sorted_size = 0;

void overflow_sort(uint16_t *arr, uint16_t *original, int *indices,
                   int length) {
  uint16_t next_arr[SIZE];
  uint16_t next_vals[SIZE];
  int next_indices[SIZE];
  int next_len = 0;

  for (int i = 0; i < length; i++) {
    uint16_t doubled = arr[i] * 2;
    uint16_t val = original[i];
    int idx = indices[i];

    if (doubled > THRESHOLD) {
      // Find insert position (DESC by value, ASC by index)
      int pos = 0;
      while (pos < sorted_size) {
        if (val > sorted_array[pos])
          break;
        pos++;
      }
      for (int j = sorted_size; j > pos; --j)
        sorted_array[j] = sorted_array[j - 1];
      sorted_array[pos] = val;
      sorted_size++;
    } else {
      next_arr[next_len] = doubled;
      next_vals[next_len] = val;
      next_indices[next_len] = idx;
      next_len++;
    }
  }

  if (next_len > 0) {
    overflow_sort(next_arr, next_vals, next_indices, next_len);
  }
}

int main() {
  uint16_t data[SIZE];
  int indices[SIZE];

  for (int i = 0; i < SIZE; i++) {
    data[i] = rand() % 256;
    indices[i] = i;
  }

  printf("Original: [");
  for (int i = 0; i < SIZE; i++) {
    printf("%u%s", data[i], i < SIZE - 1 ? ", " : "");
  }
  printf("]\n");

  clock_t start = clock();
  overflow_sort(data, data, indices, SIZE);
  clock_t end = clock();

  printf("Sorted by recursive overflow: [");
  for (int i = 0; i < sorted_size; i++) {
    printf("%u%s", sorted_array[i], i < sorted_size - 1 ? ", " : "");
  }
  printf("]\n");

  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Elapsed time: %.6f seconds\n", elapsed);
  return 0;
}
