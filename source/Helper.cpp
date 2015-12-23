#include "Helper.hpp"

namespace drgn
{
    std::random_device Random;
    std::mt19937 PseudoRandom(Random());
    std::uniform_real_distribution<float> UniformUnitFloatDistribution(0.0f, std::nextafter(1.0f, 2.0f));
}
