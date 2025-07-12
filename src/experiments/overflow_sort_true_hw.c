
/**
 * @file overflow_sort_true_hw.c
 * @brief True hardware-overflow sort using SSE2 and uint8_t logic.
 *
 * Sorts a uint8_t array by simulating repeated multiplications
 * and detecting overflow via hardware wraparound. Values that overflow
 * first are inserted into the output array in sorted order.
 *
 * @author Scott Douglass
 * @date 2025-07-11
 * @license MIT
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h> // SSE2

#define SIZE 16 // 16 values per __m128i
#define MAX_ITER 256

void overflow_sort_true_hw(uint8_t input[], int size, uint8_t sorted[]) {
    uint8_t values[SIZE];
    uint8_t active[SIZE] = {0}; // 0 = active, 1 = already overflowed
    memcpy(values, input, SIZE);

    uint8_t result[SIZE];
    int sorted_count = 0;

    for (int iter = 1; sorted_count < size && iter < MAX_ITER; ++iter) {
        __m128i v = _mm_loadu_si128((__m128i*)values);
        __m128i prev = v;

        // Promote to 16-bit to avoid early overflow, do mul, then narrow
        __m128i v_lo = _mm_unpacklo_epi8(v, _mm_setzero_si128());
        __m128i v_hi = _mm_unpackhi_epi8(v, _mm_setzero_si128());

        v_lo = _mm_mullo_epi16(v_lo, _mm_set1_epi16(iter));
        v_hi = _mm_mullo_epi16(v_hi, _mm_set1_epi16(iter));

        __m128i new_v = _mm_packus_epi16(v_lo, v_hi); // Re-narrow to uint8_t

        // Detect overflow: if new_v < prev_v ⇒ wraparound occurred
        __m128i overflow_mask = _mm_cmplt_epi8(new_v, prev);

        uint8_t overflow_flags[SIZE];
        _mm_storeu_si128((__m128i*)overflow_flags, overflow_mask);

        // For each overflowed value, insert into sorted[] in correct position
        for (int i = 0; i < size; i++) {
            if (!active[i] && overflow_flags[i]) {
                // Mark as overflowed
                active[i] = 1;
                // Insert input[i] into sorted[] in ascending order
                int j = sorted_count - 1;
                while (j >= 0 && input[i] < sorted[j]) {
                    sorted[j + 1] = sorted[j];
                    j--;
                }
                sorted[j + 1] = input[i];
                sorted_count++;
            }
        }

        // Save new values (for next overflow check)
        _mm_storeu_si128((__m128i*)values, new_v);
    }
}

int main() {
    uint8_t input[SIZE] = {5, 2, 6, 1, 9, 3, 8, 7, 10, 0, 12, 11, 4, 14, 13, 15};
    uint8_t sorted[SIZE] = {0};

    overflow_sort_true_hw(input, SIZE, sorted);

    printf("Original: ");
    for (int i = 0; i < SIZE; i++) printf("%d ", input[i]);
    printf("\nSorted:   ");
    for (int i = 0; i < SIZE; i++) printf("%d ", sorted[i]);
    printf("\n");

    return 0;
}
