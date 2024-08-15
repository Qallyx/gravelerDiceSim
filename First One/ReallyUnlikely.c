#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

static uint32_t xorshift32(uint32_t *seed) {
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

clock_t start = clock();

for (i = 0; i < 1000000000; i++) {
        int check = 0;

        for (j = 0; j < 21; j++){
            rolls = xorshift32(&seed);
            for (k = 0; k < 11; k++){
                rolls = rolls >> 2;
                if ((rolls & 0x03) == 0){
                    check++;
                }
            }
        }

        if (check > closest) {
            closest = check;
            printf("%d at roll %d\n", closest, i + 1);
        }
    }

clock_t stop = clock();

printf("Most ones = %d\n", closest);

float elapsed_time = (stop - start) * 1000 / CLOCKS_PER_SEC;

printf("Finished in %f", elapsed_time);

getchar();

}

//177/231