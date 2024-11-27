#include "flock.hpp"
#include "arena.hpp"
#include "boid.hpp"

extern int speedForceRandomDampener;


threepp::Vector3 Flock::flockCalculateSeparation(const Boid& boid) {
    threepp::Vector3 separationForce(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (const Boid* neighbor : arena.getNeighboringBoids(boid, separationRadius / arena.getCellSize())) {
        threepp::Vector3 displacement = boid.boidGetPosition() - neighbor->boidGetPosition();
        float distance = displacement.length();
        if (distance < separationRadius && distance > 0) {
            displacement.normalize();
            separationForce += (displacement) / (distance * distance);
            count++;
        }
    }
    if (count > 0) separationForce /= static_cast<float>(count);
    separationForce *= separationStrength;
    return separationForce;
}

threepp::Vector3 Flock::flockCalculateAlignment(const Boid& boid) {
    threepp::Vector3 alignmentForce(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (const Boid* neighbor : arena.getNeighboringBoids(boid, alignmentRadius / arena.getCellSize())) {
        float distance = (boid.boidGetPosition() - neighbor->boidGetPosition()).length();
        if (distance < alignmentRadius) {
            alignmentForce += neighbor->boidGetVelocity();
            count++;
        }
    }
    if (count > 0) {
        alignmentForce /= static_cast<float>(count);  // Average the velocities
        if (alignmentForce.length() > 0) {
            alignmentForce.normalize();  // Normalize to get a direction vector
        }
    }

    alignmentForce *= alignmentStrength;
    return alignmentForce;
}

threepp::Vector3 Flock::flockCalculateCohesion(const Boid& boid) {
    threepp::Vector3 cohesionForce(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (const Boid* neighbor : arena.getNeighboringBoids(boid, cohesionRadius / arena.getCellSize())) {
        float distance = (boid.boidGetPosition() - neighbor->boidGetPosition()).length();
        if (distance < cohesionRadius) {
            cohesionForce += neighbor->boidGetPosition();
            count++;
        }
    }
    if (count > 0) {
        cohesionForce /= static_cast<float>(count);
        cohesionForce -= boid.boidGetPosition();
        cohesionForce.normalize();
    }
    cohesionForce *= cohesionStrength;
    return cohesionForce;
}

void Flock::flockAddBoid(std::unique_ptr<Boid> boid) {
    boids.push_back(std::move(boid));
}

void Flock::flockApplyFlockingForces() {
    arena.clearGrid();
    for (const auto& boid : boids) {
        arena.addBoid(boid.get());
    }

    float dampener = 0.1f;
    float scaredDampener = 0.5f;

    for (auto& boid : boids) {
        threepp::Vector3 separationForce = flockCalculateSeparation(*boid);
        threepp::Vector3 alignmentForce = flockCalculateAlignment(*boid);
        threepp::Vector3 cohesionForce = flockCalculateCohesion(*boid);

        if (boid->boidGetBoidScaredCheck()) {

            separationForce *= scaredDampener;
            alignmentForce *= scaredDampener;
            cohesionForce *= scaredDampener;
        }

        if(flockGetBoidOutOfBoundsCheck(boid) == true) {

            cohesionForce *= dampener;
            separationForce *= dampener;
            alignmentForce *= dampener;
        }


        boid->boidApplyForce(separationForce * dampener);
        boid->boidApplyForce(alignmentForce * dampener);
        boid->boidApplyForce(cohesionForce * dampener);
    }
}

void Flock::flockUpdateFlock() {
    flockApplyFlockingForces();
    for (auto& boid : boids) {
        boid->boidUpdateBoid();
    }
}

const std::vector<std::unique_ptr<Boid>>& Flock::getBoids() const {
    return boids;
}

const Boid& Flock::getBoidByIndex(int index) const {
    return *boids[index];
}

const int Flock::flockGetNumBoids() {
    return static_cast<int>(boids.size());
}

bool Flock::flockGetBoidOutOfBoundsCheck(const std::unique_ptr<Boid>& boid) const {
    extern float borderInvisiblePercentage;

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