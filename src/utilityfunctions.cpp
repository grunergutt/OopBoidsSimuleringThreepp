#include "utilityfunctions.hpp"
#include "random"


float getRandomFloat(float min, float max) {
    static std::random_device rd;       // Static to initialize only once
    static std::mt19937 mt(rd());       // Mersenne Twister random number generator

    std::uniform_real_distribution<float> dist(min, max); // Define range dynamically
    return dist(mt);  // Generate and return random number within the range
}