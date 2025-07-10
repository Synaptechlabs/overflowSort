# Makefile for Pop Sort
CC = gcc
CFLAGS = -O2
AVXFLAGS = -O2 -mavx2
SSEFLAGS = -O2 -msse4.1
LDFLAGS = -lm

SRC_DIR = src
BENCH_DIR = benchmarks
BUILD_DIR = build

SRC_FILES = \
    $(SRC_DIR)/overflow_sort_scaled.c \
    $(SRC_DIR)/overflow_sort_simd.c \
    $(SRC_DIR)/overflow_sort_avx2.c \
    $(SRC_DIR)/overflow_sort_counting.c \
    $(SRC_DIR)/uint8_t.c \
    $(SRC_DIR)/SIMD-Multiply-Sort.c

BENCH_FILES = \
    $(BENCH_DIR)/overflow_bench.c \
    $(BENCH_DIR)/overflow_vs_qsort_avx2.c \
    $(BENCH_DIR)/overflow_vs_radix_vs_qsort.c \
    $(BENCH_DIR)/sort_scaling_benchmark.c

all: build_dirs overflow_sort_scaled overflow_sort_simd overflow_sort_avx2 \
     overflow_sort_counting uint8_t SIMD-Multiply-Sort \
     overflow_bench overflow_vs_qsort_avx2 overflow_vs_radix_vs_qsort sort_scaling_benchmark

build_dirs:
	mkdir -p $(BUILD_DIR)

overflow_sort_scaled:
	$(CC) $(CFLAGS) $(SRC_DIR)/overflow_sort_scaled.c -o $(BUILD_DIR)/overflow_sort_scaled

overflow_sort_simd:
	$(CC) $(SSEFLAGS) $(SRC_DIR)/overflow_sort_simd.c -o $(BUILD_DIR)/overflow_sort_simd

overflow_sort_avx2:
	$(CC) $(AVXFLAGS) $(SRC_DIR)/overflow_sort_avx2.c -o $(BUILD_DIR)/overflow_sort_avx2

overflow_sort_counting:
	$(CC) $(CFLAGS) $(SRC_DIR)/overflow_sort_counting.c -o $(BUILD_DIR)/overflow_sort_counting

uint8_t:
	$(CC) $(CFLAGS) $(SRC_DIR)/uint8_t.c -o $(BUILD_DIR)/uint8_t

SIMD-Multiply-Sort:
	$(CC) $(CFLAGS) $(SRC_DIR)/SIMD-Multiply-Sort.c -o $(BUILD_DIR)/SIMD-Multiply-Sort

overflow_bench:
	$(CC) $(CFLAGS) $(BENCH_DIR)/overflow_bench.c -o $(BUILD_DIR)/overflow_bench $(LDFLAGS)

overflow_vs_qsort_avx2:
	$(CC) $(AVXFLAGS) $(BENCH_DIR)/overflow_vs_qsort_avx2.c -o $(BUILD_DIR)/overflow_vs_qsort_avx2

overflow_vs_radix_vs_qsort:
	$(CC) $(CFLAGS) $(BENCH_DIR)/overflow_vs_radix_vs_qsort.c -o $(BUILD_DIR)/overflow_vs_radix_vs_qsort $(LDFLAGS)

sort_scaling_benchmark:
	$(CC) $(CFLAGS) $(BENCH_DIR)/sort_scaling_benchmark.c -o $(BUILD_DIR)/sort_scaling_benchmark $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)/*
