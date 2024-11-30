#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "threepp/threepp.hpp"
#include <vector>
#include <memory>

class Boid;

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
        : separationStrength(10.0f),
          alignmentStrength(0.15f),
          cohesionStrength(0.1f),
          separationRadius(10),
          alignmentRadius(15),
          cohesionRadius(15) {}

    void flockAddBoid(std::unique_ptr<Boid> boid);
    void flockApplyFlockingForces();
    void flockUpdateFlock();

    const std::vector<std::unique_ptr<Boid>>& getBoids() const;
    const Boid& getBoidByIndex(int index) const;
    const int flockGetNumBoids();
    bool flockGetBoidOutOfBoundsCheck(const std::unique_ptr<Boid>& boid) const;

    void setSeparationStrength(float strength) { separationStrength = strength; }
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }
    void setSeparationRadius(int radius) { separationRadius = radius; }
    void setAlignmentRadius(int radius) { alignmentRadius = radius; }
    void setCohesionRadius(int radius) { cohesionRadius = radius; }

    float getSeparationStrength();
    float getAlignmentStrength();
    float getCohesionStrength();
    int getSeparationRadius();
    int getAlignmentRadius();
    int getCohesionRadius();

};

#endif
