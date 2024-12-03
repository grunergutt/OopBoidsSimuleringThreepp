#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "threepp/threepp.hpp"
#include <vector>
#include <memory>

class Boid;

class Flock {
private:
    std::vector<std::unique_ptr<Boid>> boids;                                           // vector to store instances of boid
    float separationStrength;                                                           // the strength and radius of flocking forces.
    int separationRadius;
    float alignmentStrength;
    int alignmentRadius;
    float cohesionStrength;
    int cohesionRadius;
    int flockSearchCellSize;

    threepp::Vector3 flockCalculateSeparation(const Boid& boid);                        // method declarations to calculate flocking forces
    threepp::Vector3 flockCalculateAlignment(const Boid& boid);
    threepp::Vector3 flockCalculateCohesion(const Boid& boid);

public:

    Flock()
        : separationStrength(10.0f),                                                    // initializes a flock with default values for
          alignmentStrength(0.15f),                                                     // flocking forces and radiuses
          cohesionStrength(0.1f),
          separationRadius(10),
          alignmentRadius(15),
          cohesionRadius(30) {}

    void flockAddBoid(std::unique_ptr<Boid> boid);                                      // adds an instance of boid to a flock
    void flockApplyFlockingForces();                                                    // calculates all flocking forces and adds it to a boid
    void flockUpdateFlock();                                                            // chains updates for frame by frame animation

    const std::vector<std::unique_ptr<Boid>>& getBoids() const;                         // getters
    const Boid& getBoidByIndex(int index) const;
    const int flockGetNumBoids();
    bool flockGetBoidOutOfBoundsCheck(const std::unique_ptr<Boid>& boid) const;         // checks if position is outside of invisible border

    void setSeparationStrength(float strength) { separationStrength = strength; }       // setters
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }
    void setSeparationRadius(int radius) { separationRadius = radius; }
    void setAlignmentRadius(int radius) { alignmentRadius = radius; }
    void setCohesionRadius(int radius) { cohesionRadius = radius; }

    float getSeparationStrength();                                                      // getters
    float getAlignmentStrength();
    float getCohesionStrength();
    int getSeparationRadius();
    int getAlignmentRadius();
    int getCohesionRadius();

};

#endif
