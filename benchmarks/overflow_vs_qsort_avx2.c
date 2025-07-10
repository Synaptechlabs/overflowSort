/**
 * @file overflow_vs_qsort_avx2.c
 * @brief Overflow sorting algorithm using overflow vs qsort avx2.
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

#define SIZE 10000000
#define LANES 8
#define MAX_TICKS 128

typedef struct {
    uint32_t value;
    int ticks;
    int index;
} TickResult;

int cmp_ticks(const void* a, const void* b) {
    TickResult* ta = (TickResult*)a;
    TickResult* tb = (TickResult*)b;
    if (ta->ticks == tb->ticks) return ta->index - tb->index;
    return ta->ticks - tb->ticks;
}

int cmp_uint32(const void* a, const void* b) {
    uint32_t ua = *(uint32_t*)a;
    uint32_t ub = *(uint32_t*)b;
    return (ua > ub) - (ua < ub);
}

void overflow_sort_avx2(uint32_t* input, int size, uint32_t* output) {
    TickResult* results = (TickResult*)malloc(size * sizeof(TickResult));
    int processed = 0;

    while (processed < size) {
        int count = (size - processed >= LANES) ? LANES : size - processed;

        __m256i vec = _mm256_loadu_si256((__m256i*)&input[processed]);
        __m256i prev = vec;
        __m256i ticks = _mm256_setzero_si256();
        __m256i popped = _mm256_setzero_si256();

        for (int t = 1; t < MAX_TICKS; ++t) {
            prev = vec;
            vec = _mm256_add_epi32(vec, vec); // Multiply by 2

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

    free(results);
}

int main() {
    static uint32_t input[SIZE];
    static uint32_t qsorted[SIZE];
    static uint32_t overflow_sorted[SIZE];

    srand((unsigned int)time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        input[i] = rand() % 0x7FFFFFFF;
    }

    memcpy(qsorted, input, sizeof(input));
    memcpy(overflow_sorted, input, sizeof(input));

    printf("Sorting %d integers\n", SIZE);

    clock_t start = clock();
    overflow_sort_avx2(overflow_sorted, SIZE, overflow_sorted);
    clock_t end = clock();
    double overflow_time = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    qsort(qsorted, SIZE, sizeof(uint32_t), cmp_uint32);
    end = clock();
    double qsort_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Overflow sort time: %.6f s\n", overflow_time);
    printf("qsort time        : %.6f s\n", qsort_time);

    return 0;
}
