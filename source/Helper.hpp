#ifndef __RAYNDER_HELPER_HPP__
#define __RAYNDER_HELPER_HPP__

#include <cassert>
#include <stdint.h>
#include <cstdlib>
#include <random>

#ifndef DRGN_ASSERT
#define DRGN_ASSERT(x) assert(x)
#endif // DRGN_ASSERT

#ifndef DRGN_ASSERT_NOT_IMPLEMENTED
#define DRGN_ASSERT_NOT_IMPLEMENTED() DRGN_ASSERT(false)
#endif // DRGN_ASSERT_NOT_IMPLEMENTED

#ifndef DRGN_ASSERT_UNIT_VECTOR
#define DRGN_ASSERT_UNIT_VECTOR(x) DRGN_ASSERT(0.9999f < glm::length(x) && glm::length(x) < 1.0001f)
#endif

#ifndef DRGN_ASSERT_ALMOST_ZERO
#define DRGN_ASSERT_ALMOST_ZERO(x) DRGN_ASSERT(-0.0001f < x && x < 0.0001f)
#endif

#ifndef DRGN_ARRAYSIZE
namespace
{
template <typename T, size_t N>
char(&ArraySizeHelper(T(&t)[N]))[N];
}

#define DRGN_ARRAYSIZE(x) (sizeof(ArraySizeHelper(x))) 
#endif // DRGN_ARRAYSIZE

namespace drgn
{

float const Pi = 3.14159265358979323846f;

template<typename T>
inline T Clamp(T value, T min, T max)
{
    DRGN_ASSERT(min <= max);
    return value < min ? min : (value > max ? max : value);
}

inline uint8_t Float32ToUint8(float value)
{
    return static_cast<uint8_t>(255.f * std::powf(drgn::Clamp(value, 0.f, 1.0f), 1.0f/2.2f));
}

extern std::mt19937 PseudoRandom;
extern std::uniform_real_distribution<float> UniformUnitFloatDistribution;

inline float GenerateRandomUnitFloat()
{
    return UniformUnitFloatDistribution(PseudoRandom);
}
inline int GenerateRandomInteger(int min, int max)
{
    DRGN_ASSERT(min <= max);
    std::uniform_int_distribution<int> randomInt(min, max);

    return randomInt(PseudoRandom);
}

}



#endif
