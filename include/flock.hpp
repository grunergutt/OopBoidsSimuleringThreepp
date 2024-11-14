#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "boid.hpp"
#include <vector>


class Flock {

private:

    std::vector<Boid> boids;

    float separationStrength;										    // How much each strength affects the group
    int separationRadius;                                               // How far each boid will check for other boids when seperating.
    float alignmentStrength;										    // Will create sliders in imgui to change dynamicly in real-time
    int alignmentRadius;                                                // The Radius that boids will look at other boids dricetion
    float cohesionStrength;											    // Paired setters below will change strengths here.
    int cohesionRadius;

    int flockSearchCellSize = 5;                                         // size of cubes each boid searches when calculation the forces above.

    threepp::Vector3 flockCalculateSeparation(const Boid& boid);
    threepp::Vector3 flockCalculateAlignment(const Boid& boid);
    threepp::Vector3 flockCalculateCohesion(const Boid& boid);

public:
    Flock()
        : separationStrength(1),
          alignmentStrength(1),
          cohesionStrength(1),
          separationRadius(10),
          alignmentRadius(10),
          cohesionRadius(10){}

	void flockAddBoid(const Boid& boid);
    void flockApplyFlockingForces();                                         	// Method to apply flocking principles
    void flockUpdateFlock();                                           	        // Method to update each boid in the flock
    void flockCreateAmountBoid(int amount);

    const std::vector<Boid>& getBoids() const;
    const Boid& getBoidByIndex(int index) const;
    const int flockGetNumBoids();

    void setSeparationStrength(float strength) { separationStrength = strength; }
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }

    void setSeparationRadius(int radius) { separationRadius = radius; }
    void setAlignmentRadius(int radius) { alignmentRadius = radius; }
    void setCohesionRadius(int radius) { cohesionRadius = radius; }

};

#endif // FLOCK_HPP
