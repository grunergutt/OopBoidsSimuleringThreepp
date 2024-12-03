#include "arena.hpp"
#include "boid.hpp"
#include "predator.hpp"
#include <iostream>

void Arena::clearGrid() {                                                                               // itterates over each cell
    for (auto& plane : boidGrid) {                                        // and clear any boids in them
        for (auto& row : plane) {
            for (auto& cell : row) {
                cell.clear();
            }
        }
    }
}

void Arena::clearPredatorGrid() {
    for (auto& plane : predatorGrid) {                                  // itterates over each cell
        for (auto& row : plane) {                                             // and clears any rpedators in them
            for (auto& cell : row) {
                cell.clear();
            }
        }
    }
}

std::tuple<int, int, int> Arena::getCellIndices(const threepp::Vector3& position) const {               // returns a spesific cell based on a given
    int x = std::clamp(static_cast<int>(position.x / cellSize), 0, xCells - 1);        // position
    int y = std::clamp(static_cast<int>(position.y / cellSize), 0, yCells - 1);
    int z = std::clamp(static_cast<int>(position.z / cellSize), 0, zCells - 1);
    return {x, y, z};
}

void Arena::addBoid(const Boid* boid) {
    auto [x, y, z] = getCellIndices(boid->boidGetPosition());                                   // adds a boid to boidgrid
    boidGrid[x][y][z].push_back(boid);
}

//chatgpt
std::vector<const Boid*> Arena::getNeighboringBoids(const Boid& boid, int range) const {                // searches for neighboring boids by
if(range < 0 || range >= std::max({width, height, depth})) {
    throw std::out_of_range("Arena::getNeighboringBoids, range too big");
    return std::vector<const Boid*>{};
}
    std::vector<const Boid*> neighbors;                                                                 // itterating over all neighboring cells in each
    auto [x, y, z] = getCellIndices(boid.boidGetPosition());                                   // direction based on a boids cell indency
    int xMin = std::max(0, x - range);                                                         // these lines sets the boundries on how far you should search
    int xMax = std::min(xCells - 1, x + range);
    int yMin = std::max(0, y - range);
    int yMax = std::min(yCells - 1, y + range);
    int zMin = std::max(0, z - range);
    int zMax = std::min(zCells - 1, z + range);
    for (int i = xMin; i <= xMax; ++i) {
        for (int j = yMin; j <= yMax; ++j) {
            for (int k = zMin; k <= zMax; ++k) {
                for (const Boid* neighbor : boidGrid[i][j][k]) {
                    if (neighbor != &boid) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
    if (neighbors.empty()) {
        return std::vector<const Boid*>{};
    }
    return neighbors;
}

void Arena::addPredator(const Predator* predator) {                                                     // Adds predator to predatorgrid
    auto [x, y, z] = getCellIndices(predator->predatorGetPosition());
    predatorGrid[x][y][z].push_back(predator);
}

                                                                                                        // modified version of getNeighBoringPredators
std::vector<const Predator*> Arena::getNeighboringPredators(const Predator& predator,
    int range) const {
    std::vector<const Predator*> neighbors;
    auto [x, y, z] = getCellIndices(predator.predatorGetPosition());
    int xMin = std::max(0, x - range);
    int xMax = std::min(xCells - 1, x + range);
    int yMin = std::max(0, y - range);
    int yMax = std::min(yCells - 1, y + range);
    int zMin = std::max(0, z - range);
    int zMax = std::min(zCells - 1, z + range);

    for (int i = xMin; i <= xMax; ++i) {
        for (int j = yMin; j <= yMax; ++j) {
            for (int k = zMin; k <= zMax; ++k) {
                for (const Predator* neighbor : predatorGrid[i][j][k]) {
                    if (neighbor != &predator) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
    return neighbors;
}



int Arena::getCellSize() const {                    //getters
    return cellSize;
}

int Arena::getArenaWidth() const {
    return width;
}

int Arena::getArenaHeight() const {
    return height;
}

int Arena::getArenaDepth() const {
    return depth;
}

std::vector<std::vector<std::vector<std::vector<const Boid*>>>> Arena::getBoidGrid() const {
    return boidGrid;
}
std::vector<std::vector<std::vector<std::vector<const Predator*>>>> Arena::getPredatorGrid() const {
    return predatorGrid;
}

void Arena::logBoidPositionsInGrid() const {                                                                // checks each cell in all directions
    for (int x = 0; x < xCells; ++x) {                                                                      // and checks if there is a boid there
        for (int y = 0; y < yCells; ++y) {                                                                  // if there is, it prints where and its id
            for (int z = 0; z < zCells; ++z) {                                                              // not used unless bug appears
                if (!boidGrid[x][y][z].empty()) {
                    std::cout << "Cell (" << x << ", " << y << ", " << z << ") contains:\n";
                    for (const Boid* boid : boidGrid[x][y][z]) {
                        const auto& position = boid->boidGetPosition();
                        std::cout << "  Boid ID " << boid->boidGetBoidIdentifier() << " at ("
                                  << position.x << ", " << position.y << ", " << position.z << ")\n";
                    }
                }
            }
        }
    }
}


int borderSizes = 60;

Arena arena(borderSizes, borderSizes, borderSizes, 5);      // an extern arena that can be used in other files

