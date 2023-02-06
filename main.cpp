#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

int main() {
    int rows = 100;
    int columns = 100;
    int min = 10;
    int max = 20;
    int threads;
    int count;

    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[columns];
    }

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-1000, 1000);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = distribution(generator);
        }
    }

    for (threads = 1; threads <= 8; threads++) {
        omp_set_num_threads(threads);
        count = 0;

        auto start = std::chrono::high_resolution_clock::now();

        #pragma omp parallel for reduction(+: count)
                for (int i = 0; i < rows - 1; i++) {
                    for (int j = 0; j < columns - 1; j++) {
                        if (min <= matrix[i][j] && matrix[i][j] <= max) {
                            count++;
                        }
                    }
                }

        auto stop = std::chrono::high_resolution_clock::now();
        int t = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        std::cout << "\nT: " << threads << "\nn:" << count << "\nt: " << t;

    }

    delete[] matrix;

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }

    return 0;
}