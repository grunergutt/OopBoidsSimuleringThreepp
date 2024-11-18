#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "boid.hpp"
#include <vector>
#include <memory>

class Flock {
private:
    std::vector<std::unique_ptr<Boid>> boids;
    float separationStrength;
    int separationRadius;
    float alignmentStrength;
    int alignmentRadius;
    float cohesionStrength;
    int cohesionRadius;
    int flockSearchCellSize;

    threepp::Vector3 flockCalculateSeparation(const Boid& boid);
    threepp::Vector3 flockCalculateAlignment(const Boid& boid);
    threepp::Vector3 flockCalculateCohesion(const Boid& boid);

public:
    Flock()
        : separationStrength(0.01),
          alignmentStrength(0.001),
          cohesionStrength(0.001),
          separationRadius(100),
          alignmentRadius(100),
          cohesionRadius(100) {}

    void flockAddBoid(std::unique_ptr<Boid> boid);
    void flockApplyFlockingForces();
    void flockUpdateFlock();

    const std::vector<std::unique_ptr<Boid>>& getBoids() const;
    const Boid& getBoidByIndex(int index) const;
    const int flockGetNumBoids();

    void setSeparationStrength(float strength) { separationStrength = strength; }
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }
    void setSeparationRadius(int radius) { separationRadius = radius; }
    void setAlignmentRadius(int radius) { alignmentRadius = radius; }
    void setCohesionRadius(int radius) { cohesionRadius = radius; }
};

#endif
