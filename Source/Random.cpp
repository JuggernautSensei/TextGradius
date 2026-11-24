#include "Random.h"

#include <random>

static std::mt19937 g_engine;

void RandomSeed()
{
    g_engine.seed(std::random_device {}());
}

int RandomRange(
    const int _min,
    const int _max)
{
    if (_min >= _max)
    {
        return _min;
    }
    std::uniform_int_distribution<int> dist(_min, _max - 1);
    return dist(g_engine);
}
