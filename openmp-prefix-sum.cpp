// mpic++ -o openmp-prefix-sum openmp-prefix-sum.cpp
// mpirun ./openmp-prefix-sum

#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <cstdlib>

int* prefix_scan(const int* input, size_t len);
void print_array(int* data, size_t len);

/**
 * Entry point
 *
 * @return Exit status
 */
int main() {

    int data[] = {1, 2, 0, 1, 2, 0, 1, 2};
    size_t len = 8;
    print_array(data, len);

    int* output = prefix_scan(data, len);
    print_array(output, len);

    return 0;
}

/**
 * Parallel left-associative prefix sum operation
 *
 * @param input The array of integers to perform a prefix scan half-reduction on
 * @param len   The number of items in the input array
 * @return      A pointer to a new array containing the reduced values
 */
int* prefix_scan(const int* input, size_t len) {

    // Create a copy of the input, so that we don't mutate the original input
    int* output = (int *)calloc(len, sizeof(int));
    memcpy(output, input, sizeof(int)*len);

    // 1 apart, 2 apart, 4 apart ...
    for (int apart=1; apart<=len; apart*=2) {

        // Create a copy so that we can safely do the sum in parallel by
        // removing the intra-array dependency
        int read[len];
        memcpy(read, output, sizeof(int)*len);

        // Compute the sum of pairs of values
#pragma omp parallel for
        for (int i=apart; i<=len; i++) {
            output[i] += read[i-apart];
        }

    }

    return output;

}

/**
 * Outputs an array of integers to STDOUT
 *
 * @param data  The array of integers to output
 * @param len   The number of items in data to output
 */
void print_array(int* data, size_t len) {
    for (int i=0; i<len; i++) {
        char delimit = (char) (i == len-1 ? '\n' : ',');
        printf("%d%c", data[i], delimit);
    }
}