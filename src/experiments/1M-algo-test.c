#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000
#define MAX_TICKS 64

typedef struct {
    uint8_t value;
    int tick;
} Entry;

void overflow_sort(uint8_t *input, int size, uint8_t *sorted) {
    Entry *entries = aligned_alloc(32, sizeof(Entry) * size);
    uint8_t *state = aligned_alloc(32, sizeof(uint8_t) * size);

    for (int i = 0; i < size; ++i) {
        state[i] = input[i];
        entries[i].value = input[i];
        entries[i].tick = -1;
    }

    for (int tick = 0; tick < MAX_TICKS; ++tick) {
        int i;
        for (i = 0; i + 32 <= size; i += 32) {
            __m256i vec = _mm256_loadu_si256((__m256i *)&state[i]);
            __m256i old = vec;
            vec = _mm256_add_epi8(vec, vec);
            _mm256_storeu_si256((__m256i *)&state[i], vec);

            __m256i overflow = _mm256_cmpgt_epi8(old, vec);
            uint32_t mask = _mm256_movemask_epi8(overflow);

            for (int j = 0; j < 32; ++j) {
                if ((mask >> j) & 1 && entries[i + j].tick == -1) {
                    entries[i + j].tick = tick;
                }
            }
        }

        // Handle tail end (not divisible by 32)
        for (; i < size; ++i) {
            uint8_t old = state[i];
            state[i] = old + old;
            if (old > state[i] && entries[i].tick == -1) {
                entries[i].tick = tick;
            }
        }
    }

    // Simple insertion sort based on tick
    for (int i = 1; i < size; ++i) {
        Entry key = entries[i];
        int j = i - 1;
        while (j >= 0 && entries[j].tick > key.tick) {
            entries[j + 1] = entries[j];
            j--;
        }
        entries[j + 1] = key;
    }

    for (int i = 0; i < size; ++i)
        sorted[i] = entries[i].value;

    free(entries);
    free(state);
}

int main() {
    uint8_t *input = aligned_alloc(32, SIZE);
    uint8_t *sorted = aligned_alloc(32, SIZE);

    srand((unsigned)time(NULL));
    for (int i = 0; i < SIZE; i++) {
        input[i] = rand() % 255 + 1;  // avoid zero
    }

    clock_t start = clock();
    overflow_sort(input, SIZE, sorted);
    clock_t end = clock();

    printf("Sorted %d elements in %.6f seconds\n", SIZE,
           (double)(end - start) / CLOCKS_PER_SEC);

    for (int i = 0; i < 16; ++i)
        printf("%u ", sorted[i]);
    printf("\n");

    free(input);
    free(sorted);
    return 0;
}


