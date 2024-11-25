#ifndef PACK_HPP
#define PACK_HPP

#endif //PACK_HPP

#include "threepp/threepp.hpp"
#include "predator.hpp"

class Pack {
    private:

    std::vector<std::unique_ptr<Predator>> predators;

    public:


    void packAddPredator(std::unique_ptr<Predator> predator);

    const std::vector<std::unique_ptr<Predator>>& packGetPredators() const;
    const int packGetNumPredators();
    const Predator& packGetPredatorByIndex(int index) const;

};

extern Pack pack1;