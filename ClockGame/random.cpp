#include "random.h"
#define OFFSET 1546351321
Random::Random(long seed)
{
    value = seed ^ OFFSET;
}

int Random::nextInt(int range)
{
    value = (((value >> 4) ^ OFFSET) << 6) ^ (OFFSET + 1) ;
    return value % range;
}

bool Random::nextBoolean()
{
    value = (((value >> 2) ^ OFFSET) << 1) ^ (OFFSET + 1);
    return value % 2;
}
