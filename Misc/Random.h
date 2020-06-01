#pragma once

#include <random>
#include <ctime>

namespace MTRandom
{
    //mersenne twister time based seed
    extern std::mt19937 mt;

    int getRandomInt(int lowerBound, int upperBound);

    double getRandomDouble(double lowerBound, double upperBound);
}





