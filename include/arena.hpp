#ifndef ARENA_HPP
#define ARENA_HPP

#include <vector>
#include <tuple>
#include <cmath>
#include "threepp/threepp.hpp"

class Boid;
class Predator;

class Arena {
private:
  int width, height, depth;
  int cellSize;
  int xCells, yCells, zCells;
  std::vector<std::vector<std::vector<std::vector<const Boid*>>>> boidGrid;
  std::vector<std::vector<std::vector<std::vector<const Predator*>>>> predatorGrid;

public:
  Arena(int width, int height, int depth, int cellSize)
    : width(width), height(height), depth(depth), cellSize(cellSize),
      xCells(static_cast<int>(std::ceil(static_cast<float>(width) / cellSize))),
      yCells(static_cast<int>(std::ceil(static_cast<float>(height) / cellSize))),
      zCells(static_cast<int>(std::ceil(static_cast<float>(depth) / cellSize))),
      boidGrid(xCells, std::vector<std::vector<std::vector<const Boid*>>>(yCells, std::vector<std::vector<const Boid*>>(zCells))),
      predatorGrid(xCells, std::vector<std::vector<std::vector<const Predator*>>>(yCells, std::vector<std::vector<const Predator*>>(zCells))) {}

  void clearGrid();
  void clearPredatorGrid();
  std::tuple<int, int, int> getCellIndices(const threepp::Vector3& position) const;
  void addBoid(const Boid* boid);

  std::vector<const Boid*> getNeighboringBoids(const Boid& boid, int range) const;
  void addPredator(const Predator* predator);
  std::vector<const Predator*> getNeighboringPredators(const Predator& predator, int range) const;

  int getCellSize() const;
  int getArenaWidth() const;
  int getArenaHeight() const;
  int getArenaDepth() const;

  //debug
  void logBoidPositionsInGrid() const;
};

extern Arena arena;

#endif
