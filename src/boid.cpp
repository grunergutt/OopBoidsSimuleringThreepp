#include "boid.hpp"
#include "arena.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "utilityfunctions.hpp"
#include <iostream>

float borderInvisiblePercentage = 0.6;                                 // between 0 and 1
int speedForceRandomDampener = 100;

Boid::Boid(int identifier, int sightRangeInitializer, bool outOfBoundsStatus,  bool boidScaredStatus,
           float maxSpeedInitializer, float maxForceInitializer,
           float randomFactorInitializer)
:   position{threepp::Vector3{
         getRandomFloat(-arena.getArenaWidth() / 2, arena.getArenaWidth() / 2),
         getRandomFloat(-arena.getArenaHeight() / 2, arena.getArenaHeight() / 2),
         getRandomFloat(-arena.getArenaDepth() / 2, arena.getArenaDepth() / 2)}},
    velocity{threepp::Vector3{0, 0, 0}},
    acceleration{threepp::Vector3{0.1, 0.1, 0.1}},
      fearFactor(0),
      sightRange(sightRangeInitializer),
      boidIdentifier(identifier),
      boidOutOfBounds(outOfBoundsStatus),
      boidScared(boidScaredStatus),
      maxSpeed(maxSpeedInitializer / speedForceRandomDampener),
      maxForce(maxForceInitializer / speedForceRandomDampener),
      randomForceFactor(randomFactorInitializer / speedForceRandomDampener),
      dampingFactor(1.0f) {}



void Boid::boidApplyRandomForce() {
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
}

void Boid::boidApplyForce(const threepp::Vector3& flockForce) {     // Function adds flocking forces calculated in flock class

    acceleration.add(flockForce);
    if (acceleration.length() > maxForce) {                         // Same as random force function.
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Boid::boidUpdateVelocity() {

    velocity.add(acceleration);                                      // Update velocity based on acceleration

    if (velocity.length() > maxSpeed) {                              // Limit speed to maxSpeed
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }

    velocity.multiplyScalar(dampingFactor);
}

void Boid::boidUpdatePosition() {
    position.add(velocity);                                          // Update position based on velocity

}

void Boid::boidUpdateBoid() {
    boidCalculateFearFactor();

    if (boidGetBoidOutOfBoundsCheck()) {
        float nudgeForce = maxForce/5;
        boidNudgeBoidAwayFromBorder(nudgeForce);
    }

    if (!boidGetBoidOutOfBoundsCheck()) {
        boidApplyRandomForce();
    }

    boidFleeFromPredator();

    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
}


void Boid::boidNudgeBoidAwayFromBorder(float nudgeStrength) {

    float arenaWidth = arena.getArenaWidth() / 2;
    float arenaHeight = arena.getArenaHeight() / 2;
    float arenaDepth = arena.getArenaDepth() / 2;
    float invisibleWidth = arenaWidth * borderInvisiblePercentage;
    float invisibleHeight = arenaHeight * borderInvisiblePercentage;
    float invisibleDepth = arenaDepth * borderInvisiblePercentage;

    threepp::Vector3 nudgePoint(
        getRandomFloat(-invisibleWidth, invisibleWidth),
        getRandomFloat(-invisibleHeight, invisibleHeight),
        getRandomFloat(-invisibleDepth, invisibleDepth));
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
        nudgeForce.x *= scaleX;
        nudgeForce.y *= scaleY;
        nudgeForce.z *= scaleZ;


        // Apply only the nudge force
        boidApplyForce(nudgeForce*1.5f);
    }
}


void Boid::boidFleeFromPredator() {

    if (!boidScared) {
        return;
    }

    float closestDistance = std::numeric_limits<float>::max();
    threepp::Vector3 closestPredatorPosition;
    float scaredDampener = std::pow(2.5f, fearFactor - 7) / speedForceRandomDampener;

    const std::vector<std::unique_ptr<Predator>>& predators = pack1.packGetPredators();

    for (const auto& predator : predators) {
        float distanceLength = (position - predator->predatorGetPosition()).length();
        if (distanceLength < closestDistance) {
            closestDistance = distanceLength;
            closestPredatorPosition = predator->predatorGetPosition();
        }
    }

    threepp::Vector3 fleeDirection = position - closestPredatorPosition;
    if (fleeDirection.length() == 0) {

        fleeDirection = threepp::Vector3(1.0f, 0.0f, 0.0f);
    }
    fleeDirection.normalize();

    threepp::Vector3 fleeForce = (fleeDirection * scaredDampener);

    boidApplyForce(fleeForce);
}

void Boid::boidCalculateFearFactor(){
    boidScared = false;

    if (pack1.packGetPredators().empty()) {
        fearFactor = 0;

        return;
    }

    float closestDistance = std::numeric_limits<float>::max();
    float fearAmount = 0.0f;

    const std::vector<std::unique_ptr<Predator>>& predators = pack1.packGetPredators();

    for (const auto& predator : predators) {
        float distanceLength = (position - predator->predatorGetPosition()).length();

        if (distanceLength < closestDistance) {
            closestDistance = distanceLength;
        }
    }

    if (closestDistance < sightRange) {
        float proximityFactor = 1.0f - (closestDistance / sightRange);
        fearAmount = proximityFactor * 10.0f;
    }

    fearFactor = static_cast<int>(fearAmount);
    boidScared = (fearFactor > 3.5);
}

const threepp::Vector3& Boid::boidGetPosition() const {
    return position;
}

const threepp::Vector3& Boid::boidGetVelocity() const {
    return velocity;
}

const threepp::Vector3& Boid::boidGetAcceleration() const {
    return acceleration;
}

int Boid::boidGetBoidIdentifier() const {
    return boidIdentifier;
}


bool Boid::boidGetBoidOutOfBoundsCheck() const {
    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;


    const threepp::Vector3& position = boidGetPosition();


    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {

        return true;
        }

    return false;
}


bool Boid::boidGetBoidScaredCheck() const {
    return boidScared;
}

float Boid::boidGetMaxSpeed() {
    return maxSpeed;
}
float Boid::boidGetMaxForce() {
    return maxForce;
}
float Boid::boidGetRandomForce() {
    return randomForceFactor;
}

void Boid::boidSetMaxSpeed(float setMaxSpeed) {
    maxSpeed = setMaxSpeed/speedForceRandomDampener;

    if (velocity.length() > maxSpeed) {
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }

    if (velocity.length() == 0) {
        velocity.set(1.0f, 1.0f, 0.0f);
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }
}

void Boid::boidSetMaxForce(float setMaxForce) {
    maxForce = setMaxForce/speedForceRandomDampener;

    if (acceleration.length() > maxForce) {
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }

    if (acceleration.length() == 0) {
        acceleration.set(1.0f, 1.0f, 0.0f);
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Boid::boidSetRandomForce(float setRandomForce) {
    randomForceFactor = setRandomForce/speedForceRandomDampener;
}



