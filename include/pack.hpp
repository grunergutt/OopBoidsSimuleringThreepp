#ifndef PACK_HPP
#define PACK_HPP

#endif //PACK_HPP

#include "threepp/threepp.hpp"
class Predator;
class Flock;

class Pack {
    private:

    std::vector<std::unique_ptr<Predator>> predators;

    int separationRadius;
    int separationStrength;

    public:
    Pack(int separationRadiusInitializer = 30, int separationStrengthInitializer = 300)
        : separationRadius(separationRadiusInitializer),
          separationStrength(separationStrengthInitializer){}

    void packAddPredator(std::unique_ptr<Predator> predator);

    const std::vector<std::unique_ptr<Predator>>& packGetPredators() const;
    const int packGetNumPredators();
    const Predator& packGetPredatorByIndex(int index) const;
    threepp::Vector3 packCalculateSeparation(const Predator& predator);
    void packUpdatePack(const std::vector<Flock*>& flocks);


};

extern Pack pack1;