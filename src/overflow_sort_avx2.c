/**
 * @file overflow_sort_avx2.c
 * @brief Overflow sorting algorithm using overflow sort avx2.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */



#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000
#define LANES 8
#define MAX_TICKS 128

// Store original index and value
typedef struct {
    uint32_t value;
    int ticks;
    int index;
} TickResult;

// Comparator for sorting by tick count
int cmp_ticks(const void* a, const void* b) {
    TickResult* ta = (TickResult*)a;
    TickResult* tb = (TickResult*)b;
    if (ta->ticks == tb->ticks) return ta->index - tb->index;
    return ta->ticks - tb->ticks;
}

// Overflow sort using AVX2 for uint32_t keys
void overflow_sort_avx2(uint32_t* input, int size, uint32_t* output) {
    TickResult results[SIZE];
    int processed = 0;

    while (processed < size) {
        int count = (size - processed >= LANES) ? LANES : size - processed;

        __m256i vec = _mm256_loadu_si256((__m256i*)&input[processed]);
        __m256i prev = vec;
        __m256i ticks = _mm256_setzero_si256();
        __m256i popped = _mm256_setzero_si256();

        for (int t = 1; t < MAX_TICKS; ++t) {
            prev = vec;
            vec = _mm256_add_epi32(vec, vec); // multiply by 2

            __m256i overflow_mask = _mm256_cmpgt_epi32(prev, vec);
            __m256i not_popped_mask = _mm256_cmpeq_epi32(popped, _mm256_setzero_si256());
            __m256i valid_mask = _mm256_and_si256(overflow_mask, not_popped_mask);

            __m256i tick_val = _mm256_set1_epi32(t);
            popped = _mm256_or_si256(popped, valid_mask);
            ticks = _mm256_blendv_epi8(ticks, tick_val, valid_mask);

            int pop_check[LANES];
            _mm256_storeu_si256((__m256i*)pop_check, popped);
            int all_popped = 1;
            for (int i = 0; i < count; ++i) {
                if (pop_check[i] == 0) {
                    all_popped = 0;
                    break;
                }
            }
            if (all_popped) break;
        }

        uint32_t val_buf[LANES];
        int tick_buf[LANES];
        _mm256_storeu_si256((__m256i*)val_buf, _mm256_loadu_si256((__m256i*)&input[processed]));
        _mm256_storeu_si256((__m256i*)tick_buf, ticks);

        for (int i = 0; i < count; ++i) {
            results[processed + i].value = val_buf[i];
            results[processed + i].ticks = tick_buf[i];
            results[processed + i].index = processed + i;
        }

        processed += count;
    }

    qsort(results, size, sizeof(TickResult), cmp_ticks);

    for (int i = 0; i < size; ++i) {
        output[i] = results[i].value;
    }
}

int main() {
    uint32_t input[SIZE];
    uint32_t output[SIZE];

    srand((unsigned int)time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        input[i] = rand() % 0x7FFFFFFF;
    }

    overflow_sort_avx2(input, SIZE, output);

    printf("First 16 sorted values by AVX2 overflow ticks:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%u\n", output[i]);
    }

    return 0;
}
