#ifndef __RAYNDER_HELPER_HPP__
#define __RAYNDER_HELPER_HPP__

#include <cassert>
#include <stdint.h>

#ifndef DRGN_ASSERT
#define DRGN_ASSERT(x) assert(x)
#endif // DRGN_ASSERT

#ifndef DRGN_ASSERT_NOT_IMPLEMENTED
#define DRGN_ASSERT_NOT_IMPLEMENTED() DRGN_ASSERT(false)
#endif // DRGN_ASSERT_NOT_IMPLEMENTED

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

template<typename T>
inline T clamp(T value, T min, T max)
{
    DRGN_ASSERT(min <= max);
    return value < min ? min : (value > max ? max : value);
}

inline uint8_t float32ToUint8(float value)
{
    return static_cast<uint8_t>(drgn::clamp(255.f * value, 0.f, 255.f));
}

}

#endif
