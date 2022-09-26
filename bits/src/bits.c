#include "bits.h"

/* int getBits(unsigned int x, int size, int l, int r)
{
    unsigned int mask = ((1 << (l - r + 1)) - 1) << r;
    return x & mask;
} */

int getBits(unsigned int x, int size, int l, int r)
{
    return (x << (size - 1 - l)) >> (size - 1 - l + r);
}

int getBit(unsigned int x, int size, int i)
{
    return (x << (size - 1 - i)) >> (size - 1);
}

int *getAllBits(int x)
{
    int *bits[32];
    for (int i = 0; i < 32; ++i)
    {
        bits[i] = getBit(x, 32, i);
    }
    return bits;
}

int setBits(int x, int size, int new, int l, int r)
{
    int mask = ((1 << (l - r + 1)) - 1) << r;
    return ((new << r) & mask) | (x & ~mask);
}