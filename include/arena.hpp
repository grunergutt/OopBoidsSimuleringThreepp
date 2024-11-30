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
  int width, height, depth;                                                                           //Arena size for walls
  int cellSize;                                                                                       //Arenas cell size on walls
  int xCells, yCells, zCells;                                                                         //The arenas respective amount of cells in each direction
  std::vector<std::vector<std::vector<std::vector<const Boid*>>>> boidGrid;                           //One grid storing instances of Boid
  std::vector<std::vector<std::vector<std::vector<const Predator*>>>> predatorGrid;                   //Another grid storing instances of Predator

public:
  Arena(int width, int height, int depth, int cellSize)                                               //Initializing Arena, a default arena is created in arena.cpp
    : width(width), height(height), depth(depth), cellSize(cellSize),
      xCells(static_cast<int>(std::ceil(static_cast<float>(width) / cellSize))),
      yCells(static_cast<int>(std::ceil(static_cast<float>(height) / cellSize))),
      zCells(static_cast<int>(std::ceil(static_cast<float>(depth) / cellSize))),
      boidGrid(xCells, std::vector<std::vector<std::vector<const Boid*>>>(yCells,
        std::vector<std::vector<const Boid*>>(zCells))),
      predatorGrid(xCells, std::vector<std::vector<std::vector<const Predator*>>>(yCells,
        std::vector<std::vector<const Predator*>>(zCells))) {}

  void clearGrid();                                                                                   //Removes all boids in boidgrid for new animation frame
  void clearPredatorGrid();                                                                           //Does the same for rpedators in predatorgrid
  std::tuple<int, int, int> getCellIndices(const threepp::Vector3& position) const;                   //Gets a respective cell compared to a boids/predators given position

  void addBoid(const Boid* boid);                                                                     //adds a boid to boidgrid
  std::vector<const Boid*> getNeighboringBoids(const Boid& boid, int range) const;                    //gets a boids neighbor based on its cellindency

  void addPredator(const Predator* predator);
  std::vector<const Predator*> getNeighboringPredators(const Predator& predator, int range) const;

  int getCellSize() const;
  int getArenaWidth() const;
  int getArenaHeight() const;
  int getArenaDepth() const;

  void logBoidPositionsInGrid() const;
};

extern Arena arena;

#endif
