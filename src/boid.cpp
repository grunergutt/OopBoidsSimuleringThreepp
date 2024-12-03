#include "boid.hpp"
#include "arena.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "utilityfunctions.hpp"
#include <iostream>

float borderInvisiblePercentage = 0.8;                                    // value between 0 and 1, decides how many percent of
                                                                          // of full arena to use for outofbounds checks
int speedForceRandomDampener = 100;                                       // dampens speed, maxforce and randomfactor, so main values dont
                                                                          // look to small in imgui sliders

Boid::Boid(int identifier, int sightRangeInitializer, bool outOfBoundsStatus,  bool boidScaredStatus, // boid constructor, done here because
           float maxSpeedInitializer, float maxForceInitializer,                                      // it relies on arena for random position
           float randomFactorInitializer)                                                             // and doing this in hpp creates sircular
    :   position{threepp::Vector3{                                                                    // dependencies, this goes for predator class also
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
    threepp::Vector3 randomForce(                                           // Generate a random force with each component between -0.2 and 0.2
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor)
    );

    acceleration.add(randomForce);
    if (acceleration.length() > maxForce) {                                 // Limit the random force to maxForce to keep it within acceptable range
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Boid::boidApplyForce(const threepp::Vector3& force) {                  // Function adds forces calculated in other places

    acceleration.add(force);
    if (acceleration.length() > maxForce) {                                 // Same as random force function.
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

void Boid::boidUpdateBoid() {                                        // Chains updates for animation frame by frame
    boidCalculateFearFactor();                                       // also handles logic for if boid should
                                                                     // fear and flee predators or not
    if (boidGetBoidOutOfBoundsCheck()) {                             // as well as if boid should be contained to borders
        float nudgeForce = maxForce/5;
        boidNudgeBoidAwayFromBorder(nudgeForce);
    }

    if (!boidGetBoidOutOfBoundsCheck()) {
        boidApplyRandomForce();
    }

    boidFleeFromPredator();

    boidUpdateVelocity();
    boidUpdatePosition();
}


void Boid::boidNudgeBoidAwayFromBorder(float nudgeStrength) {               // creates a force towards a random point
                                                                            // within the invisible borders of the arena
    float arenaWidth = arena.getArenaWidth() / 2;                           // and pushes boid toward it based on distance
    float arenaHeight = arena.getArenaHeight() / 2;                         // from wall
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

        boidApplyForce(nudgeForce*1.5f);
    }
}


void Boid::boidFleeFromPredator() {                                         // checks if boid attribute boidScared is true (calculated
                                                                            // in function under this), if not its not it stops.
    if (!boidScared) {                                                      // if it is it finds the closest predator and calculates a
        return;                                                             // scaling force similarly to boidnudgeawayfromborder.
    }

    float closestDistance = std::numeric_limits<float>::max();
    threepp::Vector3 closestPredatorPosition;
    float scaredDampener = std::pow(2.5f, fearFactor - 7)
    / speedForceRandomDampener;

    const std::vector<std::unique_ptr<Predator>>& predators
    = pack1.packGetPredators();

    for (const auto& predator : predators) {
        float distanceLength = (position -
            predator->predatorGetPosition()).length();
        if (distanceLength < closestDistance) {
            closestDistance = distanceLength;
            closestPredatorPosition = predator->predatorGetPosition();
        }
    }

    threepp::Vector3 fleeDirection = position - closestPredatorPosition;    // creates direction-vector away from predator
    if (fleeDirection.length() == 0) {

        fleeDirection = threepp::Vector3(1.0f, 0.0f, 0.0f);
    }
    fleeDirection.normalize();

    threepp::Vector3 fleeForce = (fleeDirection * scaredDampener);

    boidApplyForce(fleeForce);
}

void Boid::boidCalculateFearFactor(){                                       // calculates boids fear and manages boidScared and fearFactor attributes
    boidScared = false;                                                     // checks if any predators is within sightange and calculates
                                                                            // fearfactor linearly based on distance to predator
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
        float proximityFactor = 1.0f - (closestDistance / sightRange);      // linear scaling
        fearAmount = proximityFactor * 10.0f;
    }

    fearFactor = static_cast<int>(fearAmount);                              // managing fearFactor and boidSCared
    boidScared = (fearFactor > 3.5);
}

const threepp::Vector3& Boid::boidGetPosition() const {                     // getters
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


bool Boid::boidGetBoidOutOfBoundsCheck() const {                            //checks if boid is outside of invisible borders
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


bool Boid::boidGetBoidScaredCheck() const {                                 // more getters
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

void Boid::boidSetMaxSpeed(float setMaxSpeed) {                 // setters, these need to be contained with defensive
    maxSpeed = setMaxSpeed/speedForceRandomDampener;            // programming because they might be bigger than maxspeed maxforce etc.

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



