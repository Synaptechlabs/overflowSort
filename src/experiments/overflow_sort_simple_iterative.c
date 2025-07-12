#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000
#define THRESHOLD 255

typedef struct {
  uint16_t value;
  int index;
} SortedEntry;

SortedEntry *sorted_array = NULL;
int sorted_count = 0;
int sorted_capacity = 0;

void insert_sorted(uint16_t value, int index) {
  // Expand sorted array if needed
  if (sorted_count >= sorted_capacity) {
    sorted_capacity = (sorted_capacity == 0) ? 1024 : sorted_capacity * 2;
    sorted_array = realloc(sorted_array, sorted_capacity * sizeof(SortedEntry));
    if (!sorted_array) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(1);
    }
  }

  // Find insertion point
  int pos = 0;
  while (pos < sorted_count) {
    if (value > sorted_array[pos].value)
      break;
    if (value == sorted_array[pos].value && index < sorted_array[pos].index)
      break;
    pos++;
  }

  // Shift and insert
  for (int i = sorted_count; i > pos; i--) {
    sorted_array[i] = sorted_array[i - 1];
  }
  sorted_array[pos].value = value;
  sorted_array[pos].index = index;
  sorted_count++;
}

void overflow_sort(uint16_t *arr, uint16_t *orig_vals, int *indices, int size) {
  uint16_t *next_arr = malloc(size * sizeof(uint16_t));
  uint16_t *next_vals = malloc(size * sizeof(uint16_t));
  int *next_indices = malloc(size * sizeof(int));
  int next_size = 0;

  for (int i = 0; i < size; i++) {
    uint16_t doubled = arr[i] * 2;
    if (doubled > THRESHOLD) {
      insert_sorted(orig_vals[i], indices[i]);
    } else {
      next_arr[next_size] = doubled;
      next_vals[next_size] = orig_vals[i];
      next_indices[next_size] = indices[i];
      next_size++;
    }
  }

  if (next_size > 0) {
    overflow_sort(next_arr, next_vals, next_indices, next_size);
  }

  free(next_arr);
  free(next_vals);
  free(next_indices);
}

int main() {
  srand((unsigned int)time(NULL));

  uint16_t *data = malloc(SIZE * sizeof(uint16_t));
  uint16_t *orig_vals = malloc(SIZE * sizeof(uint16_t));
  int *indices = malloc(SIZE * sizeof(int));

  for (int i = 0; i < SIZE; i++) {
    data[i] = rand() % 256;
    orig_vals[i] = data[i];
    indices[i] = i;
  }

  printf("Original: [%u, %u, %u, %u, %u ...]\n", data[0], data[1], data[2],
         data[3], data[4]);

  clock_t start = clock();
  overflow_sort(data, orig_vals, indices, SIZE);
  clock_t end = clock();

  printf("Sorted by recursive overflow (first 10):\n");
  for (int i = 0; i < sorted_count && i < 10; i++) {
    printf("%u ", sorted_array[i].value);
  }
  printf("\nTime taken: %.6f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

  free(data);
  free(orig_vals);
  free(indices);
  free(sorted_array);
  return 0;
}
