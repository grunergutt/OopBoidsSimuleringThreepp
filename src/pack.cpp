#include "pack.hpp"
#include "arena.hpp"
#include <threepp/threepp.hpp>
#include "predator.hpp"
#include <iostream>

void Pack::packAddPredator(std::unique_ptr<Predator> predator){                 // adds predator to pack
    predators.push_back(std::move(predator));
}

const std::vector<std::unique_ptr<Predator>>& Pack::packGetPredators() const{   // getters
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

threepp::Vector3 Pack::packCalculateSeparation(const Predator& predator) {      // calculates a separating force between predators
                                                                                // this is very similar to flocks separation logic
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (const Predator* neighbor : arena.getNeighboringPredators(predator,     // uses neighboring rpedators to calculate a repelling force
        separationRadius / arena.getCellSize())) {                         // based on the neighboring predators position
        threepp::Vector3 displacement = predator.predatorGetPosition()
        - neighbor->predatorGetPosition();
        float distance = displacement.length();
        if (distance < separationRadius && distance > 0) {                      // normalizes for direction
            displacement.normalize();
            separationForce += (displacement/30) / (distance * distance);
            count++;
        }
    }
    if (count > 0) {
        separationForce /= static_cast<float>(count);
    }
    separationForce *= separationStrength;                                      // uses atribute to dictate strength

    return separationForce;
}

void Pack::packUpdatePack(const std::vector<Flock*>& flocks) {                  // itterates over each predator and updates it
                                                                                // using updatePredator. the parameter passed here is a
    for (auto& predator : predators) {                        // collective of flocks, since predators need to go
        arena.clearPredatorGrid();                                              // through all flocks to calculate attack point.
                                                                                // clears predator grid to stop animation from rendering
        for (const auto& predator : predators) {          // same predator multiple times
            arena.addPredator(predator.get());
        }

        predator->predatorApplyForce(packCalculateSeparation(*predator));       // calculates if predators need repulsionforce
        predator->predatorUpdatePredator(flocks);                               //updates flock
    }
}

Pack pack1;