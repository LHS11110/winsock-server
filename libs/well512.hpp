#ifndef _WELL512_H_
#define _WELL512_H_

typedef unsigned int uint;

class well512
{
protected:
    uint state[16];
    uint idx;

public:
    well512();

    well512(uint nSeed);

    uint random(int minValue, int maxValue);
    uint random(uint maxValue);
    uint random();
};

#endif