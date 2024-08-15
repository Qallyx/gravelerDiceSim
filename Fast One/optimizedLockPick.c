#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

static inline uint32_t xorshift32(uint32_t *seed) {
    uint32_t x = *seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *seed = x;
    return x;
}

int main() {
    uint32_t seed = 1234567890;
    int i, j, k, rolls, closest = 0;

    LARGE_INTEGER frequency, start, stop;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    #pragma omp parallel for private(j, k, rolls) reduction(max:closest)
    for (i = 0; i < 1000000000; i++) {
        int check = 0;
        uint32_t local_seed = seed + i;

        for (j = 0; j < 21; j++) {
            rolls = xorshift32(&local_seed);

            for (k = 0; k < 11; k++) {
                if ((rolls & 0x03) == 0) {
                    check++;
                }
                rolls >>= 2;
            }
        }

        if (check > closest) {
            closest = check;
        }
    }

    QueryPerformanceCounter(&stop);
    double elapsed_time = (double)(stop.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    
    printf("Most ones = %d\n", closest);
    printf("Finished in %f ms\n", elapsed_time);

    getchar();
}