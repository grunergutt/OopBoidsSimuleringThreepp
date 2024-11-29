


#include "predator.hpp"
#include "utilityfunctions.hpp"
#include "boid.hpp"
#include <cmath>
#include "flock.hpp"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern int speedForceRandomDampener;
extern float borderInvisiblePercentage;

Predator::Predator(int predatorIdentifier,
                   int fieldOfView,
                   bool outOfBoundsStatus,
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
    aggression(0),
    sightAngle(fieldOfView),
    sightRange(arena.getArenaDepth() / 2),
    attacking(attackingStatus),
    predatorOutOfBounds(outOfBoundsStatus),
    maxSpeed(maxSpeedInitializer / static_cast<float>(speedForceRandomDampener)),
    maxForce(maxForceInitializer / static_cast<float>(speedForceRandomDampener)),
    randomForceFactor(randomFactorInitializer / static_cast<float>(speedForceRandomDampener)),
    dampingFactor(0.99f),
    cooldownFrames(0){}


threepp::Vector3 Predator::predatorCalculateAttackPoint(const std::vector<Flock*>& flocks) {
    if (cooldownFrames > 0) {
        return threepp::Vector3(0, 0, 0);
    }

    threepp::Vector3 centerOfMass(0, 0, 0);
    int boidsInSight = 0;

    for (const auto& flock : flocks) {
        for (const auto& boid : flock->getBoids()) {
            threepp::Vector3 toBoid = boid->boidGetPosition() - position;
            float distance = toBoid.length();

            if (distance > sightRange || sightRange <= 0) {
                continue; // Skip if out of range
            }

            if (toBoid.length() == 0) {
                toBoid.set(1.0f, 0.0f, 0.0f); // Default direction for zero-length vector
            }
            toBoid.normalize();

            threepp::Vector3 forward = velocity.length() > 0 ? velocity.normalize() : threepp::Vector3(1.0f, 0.0f, 0.0f);
            float dotProduct = forward.dot(toBoid);
            dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

            float angle = std::acos(dotProduct) * (180.0f / M_PI);
            if (angle > sightAngle / 2.0f) {
                continue;
            }

            centerOfMass.add(boid->boidGetPosition());
            boidsInSight++;
        }
    }

    if (boidsInSight > 0) {
        centerOfMass.multiplyScalar(1.0f / boidsInSight);
        return centerOfMass;
    }

    return threepp::Vector3(0, 0, 0); // Default target if none found
}

void Predator::predatorAttackPoint(const threepp::Vector3& target) {
    if (target.length() == 0) {
        return; // No valid target
    }

    threepp::Vector3 direction = target - position;
    if (direction.length() == 0) {
        direction.set(1.0f, 0.0f, 0.0f); // Default direction
    } else {
        direction.normalize();
    }

    threepp::Vector3 desiredVelocity = direction * maxSpeed * 3; // Boost speed during attack
    threepp::Vector3 steering = desiredVelocity - velocity;

    if (steering.length() > maxForce * 3) {
        steering.normalize();
        steering.multiplyScalar(maxForce * 3);
    }

    acceleration.add(steering);
}


void Predator::predatorApplyRandomForce(){

    threepp::Vector3 randomForce(                                    // Generate a random force with each component between -0.2 and 0.2
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor)
    );

    acceleration.add(randomForce);
    if (acceleration.length() > maxForce) {                           // Limit the random force to maxForce to keep it within acceptable range
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
    //std::cout << "Random: " << randomForce << " Acc: " << acceleration << std::endl;  //debug
}

void Predator::predatorApplyForce(const threepp::Vector3& force) {     // Function adds flocking forces calculated in flock class

    acceleration.add(force);
    if (acceleration.length() > maxForce) {                                 // Same as random force function.
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

void Predator::predatorUpdatePredator(const std::vector<Flock*>& flocks) {
    if (predatorGetOutOfBoundsCheck()) {
        float nudgeForce = maxForce / 10.0f;
        predatorNudgePredatorAwayFromBorder(nudgeForce);
    }

    if (!predatorGetOutOfBoundsCheck()) {
        predatorApplyRandomForce();
    }

    predatorAttackPoint(predatorCalculateAttackPoint(flocks));

    predatorUpdateVelocity();
    predatorUpdatePosition();
}

void Predator::predatorNudgePredatorAwayFromBorder(float nudgeStrength) {
    extern float borderInvisiblePercentage;

    float arenaWidth = arena.getArenaWidth() / 2;
    float arenaHeight = arena.getArenaHeight() / 2;
    float arenaDepth = arena.getArenaDepth() / 2;
    float invisibleWidth = arenaWidth * borderInvisiblePercentage;
    float invisibleHeight = arenaHeight * borderInvisiblePercentage;
    float invisibleDepth = arenaDepth * borderInvisiblePercentage;

    threepp::Vector3 nudgePoint(0, 0, 0);
    threepp::Vector3 directionToOrigin = nudgePoint - position;

    float scaleX = 0.0f, scaleY = 0.0f, scaleZ = 0.0f;

    if (std::abs(position.x) > invisibleWidth) {
        float scaleFactor = (std::abs(position.x) - invisibleWidth) / (arenaWidth - invisibleWidth);
        scaleX = std::pow(scaleFactor, 5.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.y) > invisibleHeight) {
        float scaleFactor = (std::abs(position.y) - invisibleHeight) / (arenaHeight - invisibleHeight);
        scaleY = std::pow(scaleFactor, 5.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.z) > invisibleDepth) {
        float scaleFactor = (std::abs(position.z) - invisibleDepth) / (arenaDepth - invisibleDepth);
        scaleZ = std::pow(scaleFactor, 5.0f); // Quadratic scaling for stronger growth near hard border
    }

    // Calculate nudge force if outside invisible border
    if (std::abs(position.x) > invisibleWidth ||
        std::abs(position.y) > invisibleHeight ||
        std::abs(position.z) > invisibleDepth) {

        directionToOrigin.normalize();

        threepp::Vector3 nudgeForce = directionToOrigin * nudgeStrength;
        nudgeForce.x *= scaleX; // Apply scaling in x-direction
        nudgeForce.y *= scaleY; // Apply scaling in y-direction
        nudgeForce.z *= scaleZ; // Apply scaling in z-direction


        // Apply only the nudge force
        predatorApplyForce(nudgeForce);
    }
}

void Predator::predatorNudgeAwayFromPredators(float nudgeForce) {

}

const threepp::Vector3& Predator::predatorGetPosition() const{
    return position;
}

const threepp::Vector3& Predator::predatorGetVelocity() const {
    return velocity;
}

const threepp::Vector3& Predator::predatorGetAcceleration() const {
    return acceleration;
}

const bool Predator::predatorGetOutOfBoundsCheck() const {

    extern float borderInvisiblePercentage;
    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;


    const threepp::Vector3& position = predatorGetPosition();


    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {
        //std::cout << "out of bounds" << std::endl;                              //debug
        return true;
        }

    //std::cout << "not out of bounds" << std::endl;                              //debug
    return false;
}

bool Predator::predatorInProximity(const Predator& otherPredator, float range) const {

    threepp::Vector3 distanceVector = this->position - otherPredator.position;
    float distance = distanceVector.length();

    return distance < range;
}
