#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

double rd(){
    return (double) rand() / (double) RAND_MAX;
}

double rand_h(int r){
    return (double) (2 * (rd() - 0.5)) / pow(2, r + 1);
}

void diamond(double* array, int r_depth, int x, int y, int N){
    // Here is where the problem lies, I believe. This is the process
    // that adds detail to the array based on the current detail and
    // recursion depth level (although this isn't explicitly recursive
    // because it doesn't call itself, it is taken from an algorithm that
    // is).

    // The size of the square to which detail will be added
    int size = (int) (pow(2, N) / pow(2, r_depth));

    // The size of the whole map
    int full_size = (int) pow(2, N) + 1;

    #ifdef DEBUG
    printf("Whole size: %d, Step size: %d, tl location: %d\n", full_size, size, full_size * (y * size) + (x * size));
    #endif

    double tl = array[full_size * (y * size) + (x * size)];
    double tr = array[full_size * (y * size) + (x * size) + size];
    double br = array[full_size * ((y * size) + size) + (x * size) + size];
    double bl = array[full_size * ((y * size) + size) + (x * size)];

    // Square step
    // | 0.5 -> 0.6 <- 0.5 |
    // |  |             |  |
    // |  v             v  |
    // | 0.4           0.3 |
    // |  ^             ^  |
    // |  |             |  |
    // | 0.5 -> 0.9 <- 0.5 |

    double mid = array[full_size * ((y * size) + size / 2) + (x * size) + size / 2];

    // Top mid
    array[full_size * ((y * size) + size) + (x * size) + size / 2] = (tl + tr) / 2.0 + rand_h(r_depth);

    // Right mid
    array[full_size * ((y * size) + size / 2) + (x * size) + size] = (tr + br) / 2.0 + rand_h(r_depth);

    // Bottom mid
    array[full_size * ((y * size) + size) + (x * size) + size / 2] = (bl + br) / 2.0 + rand_h(r_depth);

    // Left mid
    array[full_size * ((y * size) + size / 2) + (x * size) + size] = (tl + bl) / 2.0 + rand_h(r_depth);
    
    // Center of the current section

    // | 0.5    0.6    0.5 |
    // |         |         |
    // |         v         |
    // | 0.4 -> 0.7 <- 0.3 |
    // |         ^         |
    // |         |         |
    // | 0.5    0.9    0.5 |

    double tm = array[full_size * ((y * size) + size) + (x * size) + size / 2];
    double rm = array[full_size * ((y * size) + size / 2) + (x * size) + size];
    double bm = array[full_size * ((y * size) + size) + (x * size) + size / 2];
    double lm = array[full_size * ((y * size) + size / 2) + (x * size) + size];

    array[full_size * ((y * size) + size / 2) + (x * size) + size / 2] = ((tm + rm + bm + lm) / 4.0) + rand_h(r_depth + 1);

}

double* new_map(int N){
    // Create a terrain map based on a detail level.
    
    srand(time(NULL));

    // Width of the map is equal to 2 ^ N + 1 so there is always a middle
    int WIDTH = (int) pow(2, N) + 1;

    double *to_return;

    // Allocate the memory to the array. even though this is going to be a
    // 3D heightmap, I only store it in a 1D array. This means all the data
    // is together by default and can still be accessed with normal x/y
    // coordinates with simple multiplication.
    printf("Allocating memory...\n");
    printf("WIDTH = %d\n", WIDTH);
    to_return = malloc((WIDTH) * (WIDTH) * sizeof *to_return);


    // Set each of the corners. Ideally these would be random from 0 to 1,
    // but this helps debug easier (since currently they are showing up much
    // brighter than the others (so I at least know this code works).
    to_return[0] = rd();
    to_return[WIDTH - 1] = rd();
    to_return[(WIDTH - 1) * WIDTH] = rd();
    to_return[(WIDTH - 1) * WIDTH + WIDTH - 1] = rd();

    // Here we go through the loop, once for each detail level. At each
    // detail level it adds more and more detail using the 'diamon-square'
    // method (which is just named 'diamond' here).
    printf("Entering the loop\n");
    for (int r = 0; r <= N; r++){
        printf("%d\n", r);
        for (int y = 0; y < pow(2, r); y++){
            for (int x = 0; x < pow(2, r); x++){
                diamond(to_return, r, x, y, N);
            }
        }
    }
    printf("Done with the loop.\n");

    // Normalize to bring all values between 0 and 1
    printf("Normalizing...\n");
    double max = -1000.0;
    double min = 1000.0;
    for (int y = 0; y < WIDTH; y++){
        for (int x = 0; x < WIDTH; x++){
            #ifdef DEBUG
            printf("X: %d, Y: %d, Val: %.2f\n", x, y, to_return[y * WIDTH + x]);
            #endif

            if (to_return[y * WIDTH + x] > max){
                max = to_return[y * WIDTH + x];
            }
            else if (to_return[y * WIDTH + x] < min){
                min = to_return[y * WIDTH + x];
            }
        }
    }

    for (int y = 0; y < WIDTH; y++){
        for (int x = 0; x < WIDTH; x++){
            to_return[y * WIDTH + x] = (to_return[y * WIDTH + x] - min) / (max - min);
        }
    }
    printf("MIN: %.2f, MAX: %.2f\n", min, max);

    printf("Done Normalizing.\n");

    // Return the final array.
    return to_return;
}

int main() {
    for (int i = 0; i < 10; i++){
        double* a = new_map(9);
    }
    return 0;
}
