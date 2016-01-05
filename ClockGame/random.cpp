#include "random.h"
#include <QtGlobal>
#define OFFSET 15463513211567
Random::Random(long seed)
{
    value = seed ^ OFFSET;
    qsrand(value);
}

int Random::nextInt(int range)
{
    value = (((value >> 4) ^ OFFSET) << 6) ^ (OFFSET + 1) + qrand();
    return value % range;
}

bool Random::nextBoolean()
{
    value = (((value >> 2) ^ OFFSET) << 1) ^ (OFFSET + 1) + qrand();
    return value % 2;
}
