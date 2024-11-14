#ifndef ARENA_HPP
#define ARENA_HPP


#include <vector>
#include <tuple>
#include <cmath>
#include "threepp/threepp.hpp"

class Boid;

class Arena {
    private:
    int width, height, depth;
    int cellSize;
    int xCells, yCells, zCells;

    std::vector<std::vector<std::vector<std::vector<const Boid*>>>> grid;


    public:

  Arena(int width, int height, int depth, int cellSize)
    : width(width), height(height), depth(depth), cellSize(cellSize),
      xCells(static_cast<int>(std::ceil(static_cast<float>(width) / cellSize))),
      yCells(static_cast<int>(std::ceil(static_cast<float>(height) / cellSize))),
      zCells(static_cast<int>(std::ceil(static_cast<float>(depth) / cellSize))),
      grid(xCells, std::vector<std::vector<std::vector<const Boid*>>>(yCells, std::vector<std::vector<const Boid*>>(zCells))) {}

  std::tuple<int, int, int> getCellIndices(const threepp::Vector3& position) const;

  void clearGrid();
  void addBoid(const Boid* boid);

  std::vector<const Boid*> getNeighboringBoids(const Boid& boid, int range) const;

  int getCellSize() const;
  int getArenaWidth() const;
  int getArenaHeight() const;
  int getArenaDepth() const;

};
int dimensions = 100;

Arena arena(dimensions, dimensions, dimensions, 5);

#endif //ARENA_HPP
