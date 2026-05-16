/**
 *Реализация решета Эратосфена: обычного и сегментного.
 */

#include "sieve.h"
#include <cmath>

namespace prime {

// Обычное решето — находит все простые числа от 2 до limit.
std::vector<uint64_t> sequentialSieve(uint64_t limit)
{
    if (limit < 2) return {};

    // is_prime[i] = true означает, что i — простое
    std::vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            // Начинаем вычёркивать с p*p — всё меньше уже вычеркнуто
            for (uint64_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    std::vector<uint64_t> primes;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes.push_back(i);

    return primes;
}

// Сегментное решето — находит простые в диапазоне [low, high].
// Каждый MPI-процесс вызывает эту функцию для своего куска.
std::vector<uint64_t> segmentedSieve(uint64_t low,
                                     uint64_t high,
                                     const std::vector<uint64_t>& primes)
{
    if (low < 2) low = 2;
    if (low > high) return {};

    std::size_t segSize = static_cast<std::size_t>(high - low + 1);
    std::vector<bool> is_prime_seg(segSize, true);

    for (uint64_t p : primes) {
        if (p * p > high) break;

        // Первое кратное p, которое >= low
        uint64_t first = ((low + p - 1) / p) * p;
        if (first == p) first += p; // само p — простое, не трогаем

        // Вычёркиваем кратные
        for (uint64_t j = first; j <= high; j += p)
            is_prime_seg[static_cast<std::size_t>(j - low)] = false;
    }

    std::vector<uint64_t> result;
    for (std::size_t i = 0; i < segSize; ++i)
        if (is_prime_seg[i])
            result.push_back(low + static_cast<uint64_t>(i));

    return result;
}

}
