#include "predator.hpp"
#include "utilityfunctions.hpp"
#include "boid.hpp"
#include "arena.hpp"
#include <cmath>
#include "flock.hpp"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern int speedForceRandomDampener;                    // same as boid, dampens force so paramaters in imgui are not too small
extern float borderInvisiblePercentage;                 // same border width as boid so they move in same arena parameters

Predator::Predator(int predatorIdentifier,              // predator constructor is placed here for same reason
                   int fieldOfView,                     // as boid, becuse it relies on arena. this might create sircluar
                   bool outOfBoundsStatus,              // dependency if used in hpp file.
                   bool attackingStatus,
                   float maxSpeedInitializer,
                   float maxForceInitializer,
                   float randomFactorInitializer)
    : position{threepp::Vector3{
        getRandomFloat(-arena.getArenaWidth() / 2, arena.getArenaWidth() / 2),
        getRandomFloat(-arena.getArenaHeight() / 2, arena.getArenaHeight() / 2),
        getRandomFloat(-arena.getArenaDepth() / 2, arena.getArenaDepth() / 2)}},
    velocity{threepp::Vector3{0, 0, 0}},
    acceleration{threepp::Vector3{0.1, 0.1, 0.1}},
    identifier(predatorIdentifier),
    sightAngle(fieldOfView),
    sightRange(arena.getArenaDepth() / 2),
    attacking(attackingStatus),
    predatorOutOfBounds(outOfBoundsStatus),
    maxSpeed(maxSpeedInitializer / static_cast<float>(speedForceRandomDampener)),
    maxForce(maxForceInitializer / static_cast<float>(speedForceRandomDampener)),
    randomForceFactor(randomFactorInitializer / static_cast<float>(speedForceRandomDampener)),
    dampingFactor(0.99f){}


//slight gpt for help with logic
threepp::Vector3 Predator::predatorCalculateAttackPoint(                    // calculates attack point by going through all boids within
    const std::vector<Flock*>& flocks) {                                    // sight angle and range and getting their average position

    threepp::Vector3 centerOfMass(0, 0, 0);
    int boidsInSight = 0;

    for (const auto& flock : flocks) {                           // itterates through flocks and then
        for (const auto& boid : flock->getBoids()) {     // itterates through boids

            threepp::Vector3 toBoid = boid->boidGetPosition() - position;   // gets the distance from predator to boid
            float distance = toBoid.length();

            if (distance > sightRange || sightRange <= 0) {
                continue;                                                   // Skip if out of range
            }

            if (toBoid.length() == 0) {
                toBoid.set(1.0f, 0.0f, 0.0f);                         // Default direction for zero-length vector
            }
            toBoid.normalize();                                             // normalizes for directional vector

            threepp::Vector3 forward = velocity.length() > 0 ?              // Checks if the velocity vector has a non-zero
                velocity.normalize()                                        // magnitude. If true, the direction of the velocity
            : threepp::Vector3(1.0f, 0.0f, 0.0f);                     // is set to a false direction to avoid division by zero

            float dotProduct = forward.dot(toBoid);
            dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

            float angle = std::acos(dotProduct) * (180.0f / M_PI);          // checks if boid is within sightangle
            if (angle > sightAngle / 2.0f) {
                continue;
            }

            centerOfMass.add(boid->boidGetPosition());                      // if a boid is within range and angle, add its position
            boidsInSight++;                                                 // and make count go up by 1
        }
    }

    if (boidsInSight > 0) {
        centerOfMass.multiplyScalar(1.0f / boidsInSight);
        return centerOfMass;
    }

    return threepp::Vector3(0, 0, 0);                                  // Default target if no boids are found
}

void Predator::predatorAttackPoint(const threepp::Vector3& target) {         // makes predator move in direction of
    if (target.length() == 0) {                                              // a Vector3 target, (predatorCalculateAttackPoint)

        return;                                                              // if the attack point is where the predator is, do nothing
    }

    threepp::Vector3 direction = target - position;
    if (direction.length() == 0) {
        direction.set(1.0f, 0.0f, 0.0f);                               // Default direction, defensive check in case direction
    } else {                                                                 // is zero
        direction.normalize();
    }

    threepp::Vector3 desiredVelocity = direction * maxSpeed * 3;            // Boost speed during attack
    threepp::Vector3 steering = desiredVelocity - acceleration;                 // calculates where the predator should be moving

    if (steering.length() > maxForce * 3) {                                 // makes rpedator move faster when valid target is around
        steering.normalize();
        steering.multiplyScalar(maxForce * 3);
    }

    acceleration.add(steering);                                             // adds the steering force to acceleration
}


void Predator::predatorApplyRandomForce(){                                  // Generate a random force based on randomforcefactor

    threepp::Vector3 randomForce(
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor)
    );

    acceleration.add(randomForce);
    if (acceleration.length() > maxForce) {                           // Limit the random force to maxForce to keep it within acceptable range
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }

}

void Predator::predatorApplyForce(const threepp::Vector3& force) {     // Function adds forces calculated elsewhere

    acceleration.add(force);
    if (acceleration.length() > maxForce) {                            // Same as random force function.
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Predator::predatorUpdateVelocity() {

    velocity.add(acceleration);                                      // Update velocity based on acceleration

    if (velocity.length() > maxSpeed) {                              // Limit speed to maxSpeed
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }

    velocity.multiplyScalar(dampingFactor);
}

void Predator::predatorUpdatePosition() {
    position.add(velocity);                                          // Update position based on velocity

}

void Predator::predatorUpdatePredator(const std::vector<Flock*>& flocks) {  // chains steps so predator moves
    if (predatorGetOutOfBoundsCheck()) {                                    // parameter here is flocks so predatorcalculateattackpoint
        float nudgeForce = maxForce / 10.0f;                                // has all targets neccesary
        predatorNudgePredatorAwayFromBorder(nudgeForce);
    }

    if (!predatorGetOutOfBoundsCheck()) {
        predatorApplyRandomForce();
    }

    predatorAttackPoint(predatorCalculateAttackPoint(flocks));

    predatorUpdateVelocity();
    predatorUpdatePosition();
}

void Predator::predatorNudgePredatorAwayFromBorder(float nudgeStrength) {   // this is an excapt copy of boidNudgeawayfromborder
    extern float borderInvisiblePercentage;                                 // except it calculates a force for predators

    float arenaWidth = arena.getArenaWidth() / 2;
    float arenaHeight = arena.getArenaHeight() / 2;
    float arenaDepth = arena.getArenaDepth() / 2;
    float invisibleWidth = arenaWidth * borderInvisiblePercentage*borderInvisiblePercentage;
    float invisibleHeight = arenaHeight * borderInvisiblePercentage*borderInvisiblePercentage;
    float invisibleDepth = arenaDepth * borderInvisiblePercentage*borderInvisiblePercentage;

    threepp::Vector3 nudgePoint(0, 0, 0);
    threepp::Vector3 directionToOrigin = nudgePoint - position;

    float scaleX = 0.0f, scaleY = 0.0f, scaleZ = 0.0f;

    if (std::abs(position.x) > invisibleWidth) {
        float scaleFactor = (std::abs(position.x) - invisibleWidth) / (arenaWidth - invisibleWidth);
        scaleX = std::pow(scaleFactor, 5.0f);
    }

    if (std::abs(position.y) > invisibleHeight) {
        float scaleFactor = (std::abs(position.y) - invisibleHeight) / (arenaHeight - invisibleHeight);
        scaleY = std::pow(scaleFactor, 5.0f);
    }

    if (std::abs(position.z) > invisibleDepth) {
        float scaleFactor = (std::abs(position.z) - invisibleDepth) / (arenaDepth - invisibleDepth);
        scaleZ = std::pow(scaleFactor, 5.0f);
    }

    if (std::abs(position.x) > invisibleWidth ||
        std::abs(position.y) > invisibleHeight ||
        std::abs(position.z) > invisibleDepth) {

        directionToOrigin.normalize();

        threepp::Vector3 nudgeForce = directionToOrigin * nudgeStrength;
        nudgeForce.x *= scaleX;
        nudgeForce.y *= scaleY;
        nudgeForce.z *= scaleZ;

        predatorApplyForce(nudgeForce);
    }
}

const threepp::Vector3& Predator::predatorGetPosition() const{      // getters
    return position;
}

const threepp::Vector3& Predator::predatorGetVelocity() const {
    return velocity;
}

const threepp::Vector3& Predator::predatorGetAcceleration() const {
    return acceleration;
}

const bool Predator::predatorGetOutOfBoundsCheck() const {              //out of bounds check for predator

    extern float borderInvisiblePercentage;
    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;


    const threepp::Vector3& position = predatorGetPosition();


    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {

        return true;
        }

    return false;
}
