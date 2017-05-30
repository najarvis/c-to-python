#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

double rd(){
    return (double) rand() / (double) RAND_MAX;
}

double rand_h(int r){
    return (double) (2 * (rd() - 0.5)) / pow(2, r+1);
}

void diamond(double** array, int r_depth, int x, int y, int N){
    int size = (int) (pow(2, N) / pow(2, r_depth));

    double tl = array[size * x][size * y];
    double tr = array[size * x + size][size * y];
    double br = array[size * x + size][size * y + size];
    double bl = array[size * x][size * y + size];

    array[size * x + size / 2][size * y + size / 2] = ((tl + tr + br + bl) / 4) + rand_h(r_depth + 1);

    // Square step
    
    double mid = array[size * x + size / 2][size * y + size / 2];

    // Top mid
    array[size * x + size / 2][size * y] = (tl + tr + mid) / 3.0 + rand_h(r_depth);

    // Right mid
    array[size * x + size][size * y + size / 2] = (tr + br + mid) / 3.0 + rand_h(r_depth);

    // Bottom mid
    array[size * x + size / 2][size * y + size] = (bl + br + mid) / 3.0 + rand_h(r_depth);

    // Left mid
    array[size * x + size][size * y + size / 2] = (tl + bl + mid) / 3.0 + rand_h(r_depth);
}

double** new_map(int N){
    int WIDTH = (int) pow(2, N);

    //double to_return[WIDTH + 1][WIDTH + 1];
    double **to_return;

    printf("Allocating memory...\n");
    to_return = malloc((WIDTH + 1) * sizeof *to_return);
    for (int i = 0; i < (WIDTH + 1); i++){
        to_return[i] = malloc((WIDTH + 1) * sizeof *to_return[i]);
    }

    srand(time(NULL));

    to_return[0][0] = rd();
    to_return[WIDTH][0] = rd();
    to_return[WIDTH][WIDTH] = rd();
    to_return[0][WIDTH] = rd();

    printf("Entering the loop\n");
    for (int r = 0; r < N; r++){
        printf("%d\n", r);
        for (int y = 0; y < pow(2, r); y++){
            for (int x = 0; x < pow(2, r); x++){
                // printf("starting the diamond step, %d, %d, %d\n", r, x, y);
                diamond(to_return, r, x, y, N);
            }
        }
    }
    printf("Done with the loop.\n");

    return to_return;
}
