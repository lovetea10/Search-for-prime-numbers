#pragma once

#include <vector>
#include <cstdint>

namespace prime {

// Обычное решето Эратосфена — для получения базовых простых до sqrt(N)
std::vector<uint64_t> sequentialSieve(uint64_t limit);

// Сегментное решето для диапазона [low, high]
std::vector<uint64_t> segmentedSieve(uint64_t low,
                                     uint64_t high,
                                     const std::vector<uint64_t>& primes);

}
