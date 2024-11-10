#include "Arena.hpp"
#include "Flock.hpp"



void Flock::flockAddBoid(const Boid& boid) {
    boids.push_back(boid);
}

threepp::Vector3 Flock::flockCalculateSeparation(const Boid& boid) {
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid* neighbor : arena.getNeighboringBoids(boid, separationRadius / arena.getCellSize())) {
        threepp::Vector3 displacement = boid.boidGetPosition() - neighbor->boidGetPosition();
        float distance = displacement.length();

        if (distance < separationRadius && distance > 0) {
            displacement.normalize();
            separationForce += displacement / (distance * distance);
            count++;
        }
    }

    if (count > 0) {
        separationForce /= static_cast<float>(count);
    }

    separationForce *= separationStrength;
    return separationForce;
}

threepp::Vector3 Flock::flockCalculateAlignment(const Boid& boid) {
    threepp::Vector3 alignmentForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Boid* neighbor : arena.getNeighboringBoids(boid, alignmentRadius / arena.getCellSize())) {
        float distance = (boid.boidGetPosition() - neighbor->boidGetPosition()).length();

        if (distance < alignmentRadius) {
            alignmentForce += neighbor->boidGetVelocity();  // Sum up velocities of nearby boids
            count++;
        }
    }

    if (count > 0) {
        alignmentForce /= static_cast<float>(count);  // Average the velocities
        alignmentForce.normalize();  // Normalize to get a direction vector
    }

    alignmentForce *= alignmentStrength;  // Scale by alignment strength
    return alignmentForce;
}


threepp::Vector3 Flock::flockCalculateCohesion(const Boid& boid) {
    threepp::Vector3 cohesionForce(0.0f, 0.0f, 0.0f);
    int count = 0;

    // Using the global arena instance to get neighbors within the cohesion range
    for (const Boid* neighbor : arena.getNeighboringBoids(boid, cohesionRadius / arena.getCellSize())) {
        float distance = (boid.boidGetPosition() - neighbor->boidGetPosition()).length();

        if (distance < cohesionRadius) {
            cohesionForce += neighbor->boidGetPosition();  // Sum up positions of nearby boids
            count++;
        }
    }

    if (count > 0) {
        cohesionForce /= static_cast<float>(count);  // Average the positions
        cohesionForce -= boid.boidGetPosition();     // Direction towards the average position
        cohesionForce.normalize();  // Normalize to get a direction vector
    }

    cohesionForce *= cohesionStrength;  // Scale by cohesion strength
    return cohesionForce;
}

void Flock::flockApplyFlockingForces() {
    arena.clearGrid();


    for (const Boid& boid : boids) {
        arena.addBoid(&boid);
    }

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