#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Boid.hpp"
#include <unordered_map>
#include <vector>


class Flock {

private:

    std::vector<Boid> boids;
    std::unordered_map<std::pair<int, int>, std::vector<const Boid*>, boost::hash<std::pair<int, int>>> grid;

    float separationStrength;										    // How much each strength affects the group
    int separationRadius = 10;                                           // How far each boid will check for other boids when seperating.
    float alignmentStrength;										    // Will create sliders in imgui to change dynamicly in real-time
    int alignmentRadius = 10;                                           // The Radius that boids will look at other boids dricetion
    float cohesionStrength;											    // Paired setters below will change strengths here.
    int cohesionRadius = 10;

    int flockSearchCellSize  5;                                         // size of cubes each boid searches when calculation the forces above.

    threepp::Vector3 flockCalculateSeparation(const Boid& boid);
    threepp::Vector3 flockCalculateAlignment(const Boid& boid);
    threepp::Vector3 flockCalculateCohesion(const Boid& boid);

public:
    Flock(float separation, float alignment, float cohesion)          // Constructor with strengths for each behavior
        : flockSearchCellSize(),
          separationStrength(separation),
          alignmentStrength(alignment),
          cohesionStrength(cohesion) {}

    void updateGrid();
    std::pair<int, int> getCell(const threepp::Vector3& position) const;

	void flockAddBoid(const Boid& boid);
    //void flockRemoveBoid(const Boid& boid);							// not added or needed yet

	void flockApplyFlockingForces();                                         	// Method to apply flocking principles
    void flockUpdateFlock();                                           	// Method to update each boid in the flock

    const std::vector<Boid>& getBoids() const;

    void setSeparationStrength(float strength) { separationStrength = strength; }
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }

    void setSeparationRadius(int radius) { separationRadius = radius; }
    void setAlignmentRadius(int radius) { alignmentRadius = radius; }
    void setCohesionRadius(int radius) { cohesionRadius = radius; }

};



void Flock::flockAddBoid(const Boid& boid) {
    boids.push_back(boid);
}

//void Flock::flockRemoveBoid(const Boid& boid) {}					                // not added or needed yet

threepp::Vector3 Flock::flockCalculateSeparation(const Boid& boid) {

    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid& other : boids) {
        if (&boid != &other) {                                                      // Ensure it's not the same boid
            threepp::Vector3 displacement = boid.boidGetPosition() - other.boidGetPosition();
            float distance = displacement.length();

            if (distance < separationRadius && distance > 0) {                      // Within radius, not zero distance, SO IT DOESENT COUNT ITSELF
                displacement.normalize();
                separationForce += displacement / (distance * distance);
                count++;
            }
        }
    }

    if (count > 0) {
        separationForce /= static_cast<float>(count);                               // Average force
    }

    separationForce *= separationStrength;                                          // Scale by separation strength
    return separationForce;                                                         // returns threepp vector
}

threepp::Vector3 Flock::flockCalculateAlignment(const Boid& boid) {
    threepp::Vector3 alignmentForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid& other : boids) {
        if (&boid != &other) {                                                  // Ensure it's not the same boid
            float distance = (boid.boidGetPosition() - other.boidGetPosition()).length();

            if (distance < alignmentRadius) {                                   // Check if within alignment radius
                alignmentForce += other.boidGetVelocity();                      // Sum velocities
                count++;
            }
        }
    }

    if (count > 0) {
        alignmentForce /= static_cast<float>(count);                            // Average the velocities
        alignmentForce.normalize();                                             // Normalize to get a unit direction vector
    }

    alignmentForce *= alignmentStrength;                                        // Scale by alignment strength
    return alignmentForce;                                                      // returns threepp vector
}


threepp::Vector3 Flock::flockCalculateCohesion(const Boid& boid) {
    threepp::Vector3 cohesionForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid& other : boids) {
        if (&boid != &other) {                                                  // Ensure it's not the same boid
            float distance = (boid.boidGetPosition() - other.boidGetPosition()).length();

            if (distance < cohesionRadius) {                                    // Check if within cohesion radius
                cohesionForce += other.boidGetPosition();                       // Sum positions
                count++;
            }
        }
    }

    if (count > 0) {
        cohesionForce /= static_cast<float>(count);                             // Average position
        cohesionForce -= boid.boidGetPosition();                                // Vector towards the average position
        cohesionForce.normalize();                                              // Normalize to get a unit direction vector
    }

    cohesionForce *= cohesionStrength;                                          // Scale by cohesion strength
    return cohesionForce;                                                       // returns threepp vector
}

void Flock::flockApplyFlockingForces() {
    for (auto& boid : boids) {
        threepp::Vector3 separationForce = flockCalculateSeparation(boid);  // Calculate separation force
        threepp::Vector3 alignmentForce = flockCalculateAlignment(boid);    // Calculate alignment force
        threepp::Vector3 cohesionForce = flockCalculateCohesion(boid);      // Calculate cohesion force

        boid.boidApplyForce(separationForce);						  // Apply each force to the boid’s acceleration
        boid.boidApplyForce(alignmentForce);
        boid.boidApplyForce(cohesionForce);
    }
}

void Flock::flockUpdateFlock() {
    flockApplyFlockingForces();                    						 // Apply flocking forces to each boid
    for (auto& boid : boids) {
        boid.boidUpdateBoid();                   					 // Update position and velocity of each boid
    }
}

const std::vector<Boid>& Flock::getBoids() const {
    return boids;
}




#endif // FLOCK_HPP
