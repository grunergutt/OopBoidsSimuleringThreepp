#include "pack.hpp"
#include <threepp/threepp.hpp>

void Pack::packAddPredator(std::unique_ptr<Predator> predator){
    predators.push_back(std::move(predator));
}

const std::vector<std::unique_ptr<Predator>>& Pack::packGetPredators() const{
    return predators;
}

const int Pack::packGetNumPredators() {
    return static_cast<int>(predators.size());
}

const Predator& Pack::packGetPredatorByIndex(int index) const {
    if (index < 0 || index >= predators.size()) {
        throw std::out_of_range("Index is out of bounds");
    }
    return *predators[index];
}

Pack pack1;