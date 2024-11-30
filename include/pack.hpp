#ifndef PACK_HPP
#define PACK_HPP

#endif //PACK_HPP

#include "threepp/threepp.hpp"
class Predator;
class Flock;

class Pack {
    private:

    std::vector<std::unique_ptr<Predator>> predators;                                     // stores instances of predator

    int separationRadius;                                                                 // radius that predators repell eachother
    int separationStrength;                                                               // strength that predators repell eachother

    public:
    Pack(int separationRadiusInitializer = 30, int separationStrengthInitializer = 300)   // initializes a pack with separation radius and separation strength
        : separationRadius(separationRadiusInitializer),
          separationStrength(separationStrengthInitializer){}

    void packAddPredator(std::unique_ptr<Predator> predator);                             // adds an instance of predator to the predators vector

    const std::vector<std::unique_ptr<Predator>>& packGetPredators() const;               // getters
    const int packGetNumPredators();
    Predator& packGetPredatorByIndex(int index) const;

    threepp::Vector3 packCalculateSeparation(const Predator& predator);                   // calculates a separation force based on proximity
    void packUpdatePack(const std::vector<Flock*>& flocks);                               // chains updates to animate pack


};

extern Pack pack1;