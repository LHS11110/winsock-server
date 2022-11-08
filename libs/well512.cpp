#include "well512.hpp"

well512::well512()
{
    this->idx = 0;
    uint s = 512;

    for (int i = 0; i < 16; i++)
        this->state[i] = s, s += s + 73;
}

well512::well512(uint nSeed)
{
    this->idx = 0;
    uint s = nSeed;

    for (int i = 0; i < 16; i++)
        this->state[i] = s, s += s + 73;
}

uint well512::random(int minValue, int maxValue)
{
    return (uint)((random() % (maxValue - minValue)) + minValue);
}

uint well512::random(uint maxValue)
{
    return random() % maxValue;
}

uint well512::random()
{
    uint a, b, c, d;
    a = this->state[this->idx];
    c = state[(this->idx + 13) & 15];
    b = a ^ c ^ (a << 16) ^ (c << 15);
    c = this->state[(this->idx + 9) & 15];
    c ^= (c >> 11);
    a = this->state[this->idx] = b ^ c;
    d = a ^ ((a << 5) & 0xda442d24U);
    this->idx = (this->idx + 15) & 15;
    a = this->state[this->idx];
    this->state[this->idx] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

    return this->state[this->idx];
}