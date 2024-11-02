#include <Random.h>

std::mt19937 rng::RNG::m_MtGenerator = std::mt19937(std::random_device{}());