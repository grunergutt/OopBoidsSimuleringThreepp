#include "arena.hpp"
#include "boid.hpp"
#include <iostream>

void Arena::clearGrid() {
    for (auto& plane : grid) {
        for (auto& row : plane) {
            for (auto& cell : row) {
                cell.clear();
            }
        }
    }
}

std::tuple<int, int, int> Arena::getCellIndices(const threepp::Vector3& position) const {
    int x = std::clamp(static_cast<int>(position.x / cellSize), 0, xCells - 1);
    int y = std::clamp(static_cast<int>(position.y / cellSize), 0, yCells - 1);
    int z = std::clamp(static_cast<int>(position.z / cellSize), 0, zCells - 1);
    return {x, y, z};
}

void Arena::addBoid(const Boid* boid) {
    auto [x, y, z] = getCellIndices(boid->boidGetPosition());
    grid[x][y][z].push_back(boid);
}

std::vector<const Boid*> Arena::getNeighboringBoids(const Boid& boid, int range) const {
    std::vector<const Boid*> neighbors;
    auto [x, y, z] = getCellIndices(boid.boidGetPosition());
    int xMin = std::max(0, x - range);
    int xMax = std::min(xCells - 1, x + range);
    int yMin = std::max(0, y - range);
    int yMax = std::min(yCells - 1, y + range);
    int zMin = std::max(0, z - range);
    int zMax = std::min(zCells - 1, z + range);
    for (int i = xMin; i <= xMax; ++i) {
        for (int j = yMin; j <= yMax; ++j) {
            for (int k = zMin; k <= zMax; ++k) {
                for (const Boid* neighbor : grid[i][j][k]) {
                    if (neighbor != &boid) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
    return neighbors;
}

int Arena::getCellSize() const {
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

void Arena::logBoidPositionsInGrid() const {
    for (int x = 0; x < xCells; ++x) {
        for (int y = 0; y < yCells; ++y) {
            for (int z = 0; z < zCells; ++z) {
                if (!grid[x][y][z].empty()) {
                    std::cout << "Cell (" << x << ", " << y << ", " << z << ") contains:\n";
                    for (const Boid* boid : grid[x][y][z]) {
                        const auto& position = boid->boidGetPosition();
                        std::cout << "  Boid ID " << boid->boidGetBoidIdentifier() << " at ("
                                  << position.x << ", " << position.y << ", " << position.z << ")\n";
                    }
                }
            }
        }
    }
}

int borderSizes = 100;
Arena arena(borderSizes, borderSizes, borderSizes, 5);
