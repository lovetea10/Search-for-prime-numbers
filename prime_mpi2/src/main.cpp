/**
 * Параллельный поиск простых чисел с помощью MPI.
 * Запуск:
 *   mpirun -np 4 ./prime_mpi 100000000
 * Программа делит диапазон [2, N] между процессами, каждый просеивает свой кусок, master собирает итог
 * и выводит количество простых чисел и время работы.
 */

#include <mpi.h>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "sieve.h"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Номер текущего процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Всего процессов

    uint64_t N = 100'000'000;
    if (argc >= 2) {
        N = static_cast<uint64_t>(std::stoull(argv[1]));
    }

    if (rank == 0) {
        std::cout << "Поиск простых чисел до " << N
                  << " на " << size << " процессах...\n";
    }

    uint64_t sqrtN = static_cast<uint64_t>(std::ceil(std::sqrt((double)N)));
    std::vector<uint64_t> basePrimes = prime::sequentialSieve(sqrtN);

    //Деление диапазона [2, N] между процессами
    uint64_t total = N - 1;
    uint64_t chunk = total / static_cast<uint64_t>(size);
    uint64_t low   = 2 + static_cast<uint64_t>(rank) * chunk;
    uint64_t high  = (rank == size - 1) ? N : low + chunk - 1;

    double tStart = MPI_Wtime();

    // Каждый процесс просеивает свой кусок
    std::vector<uint64_t> localPrimes = prime::segmentedSieve(low, high, basePrimes);

    double tEnd = MPI_Wtime();
    double elapsed = tEnd - tStart;

    // Собираем количество простых со всех процессов на мастере
    uint64_t localCount = static_cast<uint64_t>(localPrimes.size());
    uint64_t totalCount = 0;
    MPI_Reduce(&localCount, &totalCount, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);

    //Берём максимальное время среди всех процессов (самый медленный определяет реальное время работы программы)
    double maxTime = 0.0;
    MPI_Reduce(&elapsed, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Найдено простых чисел: " << totalCount << "\n";
        std::cout << "Время выполнения:      " << maxTime << " сек\n";
    }

    MPI_Finalize();
    return 0;
}
