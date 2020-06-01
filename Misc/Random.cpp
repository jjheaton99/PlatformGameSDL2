#include "Random.h"

std::mt19937 MTRandom::mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

int MTRandom::getRandomInt(int lowerBound, int upperBound)
{
    std::uniform_int_distribution intDist{ lowerBound, upperBound };
    return intDist(mt);
}

double MTRandom::getRandomDouble(double lowerBound, double upperBound)
{
    std::uniform_real_distribution<double> doubleDist{ lowerBound, upperBound };
    return doubleDist(mt);
}
