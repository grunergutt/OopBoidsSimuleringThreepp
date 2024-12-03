#include "utilityfunctions.hpp"
#include "random"

//chatgpt for help with mersenne twister
float getRandomFloat(float min, float max) {              // returns a random floating point number
    static std::random_device rd;                         // in a given range
    static std::mt19937 mt(rd());                     // Mersenne Twister random number generator

    std::uniform_real_distribution<float> dist(min, max); // Define range dynamically
    return dist(mt);                                   // Generate and return random number within the range
}