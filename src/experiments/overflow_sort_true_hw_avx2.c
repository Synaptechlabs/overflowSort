
/**
 * @file overflow_sort_true_hw_avx2.c
 * @brief Clean crash-proof AVX2 overflow sort.
 *
 * Author: Scott Douglass
 * Date: 2025-07-11
 * License: MIT
 */

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITER 256

void overflow_sort_true_hw_avx2(uint8_t *input, int size, uint8_t *sorted) {
    uint8_t *values = malloc(size);
    uint8_t *active = calloc(size, 1);
    int sorted_count = 0;

    if (!values || !active) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(values, input, size);

    for (int iter = 1; sorted_count < size && iter < MAX_ITER; ++iter) {
        for (int i = 0; i < size; i += 32) {
            int count = (size - i >= 32) ? 32 : size - i;

            __m256i v = _mm256_loadu_si256((__m256i *)&values[i]);
            __m256i prev = v;

            __m256i lo = _mm256_unpacklo_epi8(v, _mm256_setzero_si256());
            __m256i hi = _mm256_unpackhi_epi8(v, _mm256_setzero_si256());

            __m256i mul = _mm256_set1_epi16(iter);
            lo = _mm256_mullo_epi16(lo, mul);
            hi = _mm256_mullo_epi16(hi, mul);

            __m256i new_v = _mm256_packus_epi16(lo, hi);
            __m256i overflow_mask = _mm256_cmpgt_epi8(prev, new_v);

            uint8_t flags[32];
            _mm256_storeu_si256((__m256i *)flags, overflow_mask);
            _mm256_storeu_si256((__m256i *)&values[i], new_v);

            for (int j = 0; j < count; j++) {
                int idx = i + j;
                if (idx >= size) continue;

                if (!active[idx] && flags[j]) {
                    active[idx] = 1;
                    if (sorted_count < size) {
                        int k = sorted_count - 1;
                        while (k >= 0 && input[idx] < sorted[k]) {
                            sorted[k + 1] = sorted[k];
                            k--;
                        }
                        sorted[k + 1] = input[idx];
                        sorted_count++;
                    }
                }
            }
        }
    }

    free(values);
    free(active);
}

int main() {
    const int size = 10000;
    uint8_t *input = malloc(size);
    uint8_t *sorted = malloc(size);

    if (!input || !sorted) {
        fprintf(stderr, "Failed to allocate input/sorted buffers\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < size; i++) {
        input[i] = (uint8_t)(rand() % 255 + 1);
    }

    clock_t start = clock();
    overflow_sort_true_hw_avx2(input, size, sorted);
    clock_t end = clock();

    printf("Sorted %d elements in %.6f seconds\n", size,
           (double)(end - start) / CLOCKS_PER_SEC);

    // Optional sanity check: print first 16 values
    for (int i = 0; i < 16; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(input);
    free(sorted);
    return 0;
}
