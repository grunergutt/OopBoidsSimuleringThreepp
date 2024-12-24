#include "flock.hpp"
#include "arena.hpp"
#include "boid.hpp"
#include <iostream>

extern int speedForceRandomDampener;                            // uses same dampner as boid so flocking forces dont dominate


// slight use of chatgpt for help with logic
threepp::Vector3 Flock::flockCalculateSeparation(const Boid& boid) {        // calculates the separating factor between boids with position
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);               // uses arenas getNeigboringBoids to reduce search
    int count = 0;                                                          // time so it doesent iterate over all boids every time
    for (const Boid* neighbor : arena.getNeighboringBoids(boid,
        separationRadius / arena.getCellSize())) {

        if (count < 15){
        threepp::Vector3 displacement = boid.boidGetPosition() -            // uses boids and neighbors position to calculate a
            neighbor->boidGetPosition();                                    // repulsionforce based on distance
        float distance = displacement.length();
        if (distance < separationRadius && distance > 0) {
            displacement.normalize();
            separationForce += (displacement) / (distance * distance);
            count++;                                                        // counts how many boids this is accounted for
        }
        }
    }
    if (count > 0) {
        separationForce /= static_cast<float>(count);
    }
    separationForce *= separationStrength;                                  // modifies separationForce by attribute separationStrength
    return separationForce;
}
// slight use of chatgpt for help with logic
threepp::Vector3 Flock::flockCalculateAlignment(const Boid& boid) {         // calculates the aligning factor between boids using
    threepp::Vector3 alignmentForce(0.0f, 0.0f, 0.0f);                // velocities so boids go in same direction
    int count = 0;                                                          // uses neighbors like separation does
    for (const Boid* neighbor : arena.getNeighboringBoids(boid,
        alignmentRadius / arena.getCellSize())) {

        if (count < 15) {
            float distance = (boid.boidGetPosition() -
                neighbor->boidGetPosition()).length();
            if (distance < alignmentRadius) {
                alignmentForce += neighbor->boidGetVelocity();
                count++;
            }
        }
    }
    if (count > 0) {
        alignmentForce /= static_cast<float>(count);                        // Average the velocities
        if (alignmentForce.length() > 0) {
            alignmentForce.normalize();                                     // Normalize to get a direction vector
        }
    }

    alignmentForce *= alignmentStrength;                                    // use attribute to scale vector
    return alignmentForce;
}
// slight use of chatgpt for help with logic
threepp::Vector3 Flock::flockCalculateCohesion(const Boid& boid) {          // calculates the cohesive force between boids
    threepp::Vector3 cohesionForce(0.0f, 0.0f, 0.0f);                 // uses position like separation, but creates the force
    int count = 0;                                                          // toward nearby boids using its neighbors.
    for (const Boid* neighbor : arena.getNeighboringBoids(boid,
        cohesionRadius / arena.getCellSize())) {

        if (count < 15) {
            float distance = (boid.boidGetPosition() -
                neighbor->boidGetPosition()).length();
            if (distance < cohesionRadius) {
                cohesionForce += neighbor->boidGetPosition();
                count++;
            }
        }
    }
    if (count > 0) {                                                        // gets the direction vector by normalizing
        cohesionForce /= static_cast<float>(count);
        cohesionForce -= boid.boidGetPosition();
        cohesionForce.normalize();
    }
    cohesionForce *= cohesionStrength;                                      // uses attribute to modify the strength og the force
    return cohesionForce;
}

void Flock::flockAddBoid(std::unique_ptr<Boid> boid) {                      // adds boid to flock
    boids.push_back(std::move(boid));
}

void Flock::flockApplyFlockingForces() {                                    // itterates over each boid calculating its respective
    arena.clearGrid();                                                      // force for each cohesion, separation and alignment
    for (const auto& boid : boids) {                     // clears the boidbrid in arena before each flock update
        arena.addBoid(boid.get());                                          // so boids dont multiply in animation loop.
    }

    float outOfBoundsDampener = 0.2f;                                       // dampens flocking forces so they dont dominate when
    float scaredDampener = 0.02;                                            // boid is out of bounds or flees predator, so that
                                                                            // they stay within borders and dont ignore rpedators

    for (auto& boid : boids) {                               // itteration
        threepp::Vector3 separationForce = flockCalculateSeparation(*boid);
        threepp::Vector3 alignmentForce = flockCalculateAlignment(*boid);
        threepp::Vector3 cohesionForce = flockCalculateCohesion(*boid);

        if (boid->boidGetBoidScaredCheck()) {                               // logic for fear check

            separationForce *= scaredDampener;
            alignmentForce *= scaredDampener;
            cohesionForce *= scaredDampener;
        }

        if(flockGetBoidOutOfBoundsCheck(boid)) {                            // logic for out of boudns check

            cohesionForce *= outOfBoundsDampener;
            separationForce *= outOfBoundsDampener;
            alignmentForce *= outOfBoundsDampener;
        }


        boid->boidApplyForce(separationForce * outOfBoundsDampener);        // applies force
        boid->boidApplyForce(alignmentForce * outOfBoundsDampener);
        boid->boidApplyForce(cohesionForce * outOfBoundsDampener);
    }
}

void Flock::flockUpdateFlock() {                                            // updates flock using updateflockingforces, then updates each boid.
    flockApplyFlockingForces();                                             // using boids updateBoid function, so they change position etc.
    for (auto& boid : boids) {
        boid->boidUpdateBoid();
    }
}

const std::vector<std::unique_ptr<Boid>>& Flock::getBoids() const {         // getters
    return boids;
}

const Boid& Flock::getBoidByIndex(int index) const {
    return *boids[index];
}

const int Flock::flockGetNumBoids() {
    return static_cast<int>(boids.size());
}

bool Flock::flockGetBoidOutOfBoundsCheck(const std::unique_ptr<Boid>& boid) const {     // simple modified version og boidGetoutofbounds
    extern float borderInvisiblePercentage;                                             // had issue where flock couldt use boids existing one

    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;

    const threepp::Vector3& position = boid->boidGetPosition();

    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {
        return true;
        }

    return false;
}

float Flock::getSeparationStrength() {
    return separationStrength;
}
float Flock::getAlignmentStrength() {
    return alignmentStrength;
}
float Flock::getCohesionStrength() {
    return cohesionStrength;
}
int Flock::getSeparationRadius() {
    return separationRadius;
}
int Flock::getAlignmentRadius() {
    return alignmentRadius;
}
int Flock::getCohesionRadius() {
    return cohesionRadius;
}