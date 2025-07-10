/**
 * @file sort_scaling_benchmark.c
 * @brief Overflow sorting algorithm using sort scaling benchmark.
 *
 * @author Scott Douglass
 * @date 2025-07-10
 * @license MIT
 */



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_SIZE 10000000
#define MAX_ROUNDS 32
#define THRESHOLD 65535

typedef struct {
    uint16_t value;
    int tick;
} OverflowEntry;

void overflow_sort_counting(uint16_t input[], int size, OverflowEntry sorted[]) {
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
        if (popped_this_round == 0) break;
    }

    for (int i = 0; i < size; ++i)
        if (temp[i].tick >= 0)
            count[temp[i].tick]++;

    for (int i = 1; i < size; ++i)
        count[i] += count[i - 1];

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

int compare_uint16(const void *a, const void *b) {
    return (*(uint16_t *)a - *(uint16_t *)b);
}

void radix_sort_uint16(uint16_t arr[], int size) {
    uint16_t *output = malloc(sizeof(uint16_t) * size);
    int count[256];

    for (int shift = 0; shift < 16; shift += 8) {
        memset(count, 0, sizeof(count));
        for (int i = 0; i < size; ++i)
            count[(arr[i] >> shift) & 0xFF]++;

        for (int i = 1; i < 256; ++i)
            count[i] += count[i - 1];

        for (int i = size - 1; i >= 0; --i)
            output[--count[(arr[i] >> shift) & 0xFF]] = arr[i];

        memcpy(arr, output, sizeof(uint16_t) * size);
    }

    free(output);
}

uint16_t generate_realworld_value() {
    double u1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
    double u2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
    double z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    double val = 128 + z * 40;
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return (uint16_t)val;
}

int main() {
    srand((unsigned int)time(NULL));
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("n,overflow_time,qsort_time,radix_time\n");

    for (int s = 0; s < num_sizes; ++s) {
        int n = sizes[s];

        uint16_t *input1 = malloc(sizeof(uint16_t) * n);
        uint16_t *input2 = malloc(sizeof(uint16_t) * n);
        uint16_t *input3 = malloc(sizeof(uint16_t) * n);
        OverflowEntry *sorted = malloc(sizeof(OverflowEntry) * n);

        for (int i = 0; i < n; ++i) {
            uint16_t val = generate_realworld_value();
            input1[i] = val;
            input2[i] = val;
            input3[i] = val;
        }

        clock_t start, end;

        start = clock();
        overflow_sort_counting(input1, n, sorted);
        end = clock();
        double t_overflow = (double)(end - start) / CLOCKS_PER_SEC;

        start = clock();
        qsort(input2, n, sizeof(uint16_t), compare_uint16);
        end = clock();
        double t_qsort = (double)(end - start) / CLOCKS_PER_SEC;

        start = clock();
        radix_sort_uint16(input3, n);
        end = clock();
        double t_radix = (double)(end - start) / CLOCKS_PER_SEC;

        printf("%d,%.6f,%.6f,%.6f\n", n, t_overflow, t_qsort, t_radix);

        free(input1);
        free(input2);
        free(input3);
        free(sorted);
    }

    return 0;
}
