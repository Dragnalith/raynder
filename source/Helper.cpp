#include "Helper.hpp"

namespace drgn
{
    std::mt19937 PseudoRandom(666);
    std::uniform_real_distribution<float> UniformUnitFloatDistribution(0.0f, 1.0f);
}
