#include "pack.hpp"
#include "arena.hpp"
#include <threepp/threepp.hpp>
#include "predator.hpp"
#include <iostream>

void Pack::packAddPredator(std::unique_ptr<Predator> predator){
    predators.push_back(std::move(predator));
}

const std::vector<std::unique_ptr<Predator>>& Pack::packGetPredators() const{
    return predators;
}

const int Pack::packGetNumPredators() {
    return static_cast<int>(predators.size());
}

Predator& Pack::packGetPredatorByIndex(int index) const {
    if (index < 0 || index >= predators.size()) {
        throw std::out_of_range("Index is out of bounds");
    }
    return *predators[index];
}

threepp::Vector3 Pack::packCalculateSeparation(const Predator& predator) {
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (const Predator* neighbor : arena.getNeighboringPredators(predator, separationRadius / arena.getCellSize())) {
        threepp::Vector3 displacement = predator.predatorGetPosition() - neighbor->predatorGetPosition();
        float distance = displacement.length();
        if (distance < separationRadius && distance > 0) {
            displacement.normalize();
            separationForce += (displacement) / (distance * distance);
            count++;
        }
    }
    if (count > 0) separationForce /= static_cast<float>(count);
    separationForce *= separationStrength;

    return separationForce;
}

void Pack::packUpdatePack(const std::vector<Flock*>& flocks) {

    for (auto& predator : predators) {
        arena.clearPredatorGrid();
        for (const auto& predator : predators) {
            arena.addPredator(predator.get());
        }
        predator->predatorApplyForce(packCalculateSeparation(*predator));
        predator->predatorUpdatePredator(flocks);
    }
}

Pack pack1;