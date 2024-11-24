#ifndef PACK_HPP
#define PACK_HPP

#endif //PACK_HPP

#include <threepp/threepp.hpp>
#include "predator.hpp"

class Pack {
    private:
    std::vector<std::unique_ptr<Predator>> predators;

    public:

    const std::vector<std::unique_ptr<Predator>> packGetPredators();


};