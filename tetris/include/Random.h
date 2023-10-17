#pragma once

#include <random>

namespace rng
{
    class RNG
    {
    public:
        template <typename Type>
        static inline Type GenerateRandomNumber(const Type min, const Type max) { return GenerateRandomNumberImpl(min, max, std::is_integral<Type>()); }

        template <typename T>
        static inline void ShuffleArray(std::vector<T>& arr) { std::shuffle(arr.begin(), arr.end(), m_MtGenerator); }

    private:

        template <typename Type>
        static inline Type GenerateRandomNumberImpl(const Type min, const Type max, std::true_type) 
        {
            std::uniform_int_distribution<Type> distribution(min, max);
            return distribution(m_MtGenerator);
        }

        template <typename Type>
        static inline Type GenerateRandomNumberImpl(const Type min, const Type max, std::false_type) 
        {
            std::uniform_real_distribution<Type> distribution(min, max);
            return distribution(m_MtGenerator);
        }

    private:
        static std::mt19937 m_MtGenerator;
    };

    std::mt19937 RNG::m_MtGenerator = std::mt19937(std::random_device{}());

} // namespace rng