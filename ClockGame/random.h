#ifndef RANDOM_H
#define RANDOM_H


class Random
{
public:
    Random(long seed);
    int nextInt(int range);
    bool nextBoolean();
private:
    unsigned long long value;
};

#endif // RANDOM_H
