#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Boid.hpp"


class Flock {

private:

    std::vector<Boid> boids;

    float separationStrength;										  // How much each strength affects the group
    float separationRadius;                                           // How far each boid will check for other boids when seperating.
    float alignmentStrength;										  // Will create sliders in imgui to change dynamicly in real-time
    float cohesionStrength;											  // Paired setters below will change strengths here.

    threepp::Vector3 calculateSeparation(const Boid& boid);
    threepp::Vector3 calculateAlignment(const Boid& boid);
    threepp::Vector3 calculateCohesion(const Boid& boid);

public:
    Flock(float separation, float alignment, float cohesion)          // Constructor with strengths for each behavior
        : separationStrength(separation),
          alignmentStrength(alignment),
          cohesionStrength(cohesion) {}

	void flockAddBoid(const Boid& boid);
    //void flockRemoveBoid(const Boid& boid);							// not added or needed yet

	void flockApplyFlocking();                                         	// Method to apply flocking principles
    void flockUpdateFlock();                                           	// Method to update each boid in the flock
    void flockApplyForcesToBoid();

    const std::vector<Boid>& getBoids() const;

    void setSeparationStrength(float strength) { separationStrength = strength; }
    void setAlignmentStrength(float strength) { alignmentStrength = strength; }
    void setCohesionStrength(float strength) { cohesionStrength = strength; }

};



void Flock::flockAddBoid(const Boid& boid) {
    boids.push_back(boid);
}

//void Flock::flockRemoveBoid(const Boid& boid) {}					  // not added or needed yet

threepp::Vector3 Flock::calculateSeparation(const Boid& boid) {
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid& other : boids) {
        if (&boid != &other) { // Ensure it's not the same boid
            threepp::Vector3 displacement = boid.position - other.position;
            float distance = displacement.length();

            if (distance < separationRadius && distance > 0) { // Within radius, not zero distance
                displacement.normalize();
                separationForce += displacement / (distance * distance);
                count++;
            }
        }
    }

    if (count > 0) {
        separationForce /= static_cast<float>(count); // Average force
    }

    separationForce *= separationStrength; // Scale by separation strength
    return separationForce;
}

threepp::Vector3 calculateAlignment(const Boid& boid){
    return threepp::Vector3();
}


threepp::Vector3 calculateCohesion(const Boid& boid){
    return threepp::Vector3();
}

void Flock::flockApplyFlocking() {
    for (auto& boid : boids) {
        threepp::Vector3 separationForce = calculateSeparation(boid); // Calculate separation force
        threepp::Vector3 alignmentForce = calculateAlignment(boid);   // Calculate alignment force
        threepp::Vector3 cohesionForce = calculateCohesion(boid);     // Calculate cohesion force

        separationForce.multiplyScalar(separationStrength);			  // Scale each force by its respective strength
        alignmentForce.multiplyScalar(alignmentStrength);
        cohesionForce.multiplyScalar(cohesionStrength);

        boid.boidApplyForce(separationForce);						  // Apply each force to the boid’s acceleration
        boid.boidApplyForce(alignmentForce);
        boid.boidApplyForce(cohesionForce);
    }
}

void Flock::flockUpdateFlock() {
    flockApplyFlocking();                    						 // Apply flocking forces to each boid
    for (auto& boid : boids) {
        boid.boidUpdateBoid();                   					 // Update position and velocity of each boid
    }
}

const std::vector<Boid>& Flock::getBoids() const {
    return boids;
}

#endif // FLOCK_HPP
