#include "boid.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "utilityfunctions.hpp"
#include <iostream>

float borderInvisiblePercentage = 0.7;                                 // between 0 and 1
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
      dampingFactor(0.9f) {}



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
    //std::cout << "Random: " << randomForce << " Acc: " << acceleration << std::endl;      //debug
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
        float nudgeForce = maxForce/10;
        boidNudgeBoidAwayFromBorder(nudgeForce);
    }

    if (!boidGetBoidOutOfBoundsCheck()) {
        boidApplyRandomForce();
    }

    boidFleeFromPredator();

    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
    //std::cout << "Boid maxSpeed: " << maxSpeed << "\n" << "Boid maxForce: " << maxForce << std::endl;             //debug
}

void Boid::boidConstrainToPhysicalBorders() {
    float width = arena.getArenaWidth();
    float height= arena.getArenaHeight();
    float depth = arena.getArenaDepth();

    if (position.x >= width / 2 || position.x <= -width / 2) {
        acceleration.x *= -1;
        position.x = std::clamp(position.x, -width / 2, width / 2);
    }
    if (position.y >= height / 2 || position.y <= -height / 2) {
        acceleration.y *= -1;
        position.y = std::clamp(position.y, -height / 2, height / 2);
    }
    if (position.z >= depth / 2 || position.z <= -depth / 2 ) {
        acceleration.z *= -1;
        position.z = std::clamp(position.z, -depth / 2, depth / 2);
    }
}

void Boid::boidNudgeBoidAwayFromBorder(float nudgeStrength) {

    float arenaWidth = arena.getArenaWidth() / 2;
    float arenaHeight = arena.getArenaHeight() / 2;
    float arenaDepth = arena.getArenaDepth() / 2;
    float invisibleWidth = arenaWidth * borderInvisiblePercentage;
    float invisibleHeight = arenaHeight * borderInvisiblePercentage;
    float invisibleDepth = arenaDepth * borderInvisiblePercentage;

    threepp::Vector3 nudgePoint(getRandomFloat(-invisibleWidth, invisibleWidth), getRandomFloat(-invisibleHeight, invisibleHeight), getRandomFloat(-invisibleDepth, invisibleDepth));
    threepp::Vector3 directionToOrigin = nudgePoint - position;

    float scaleX = 0.0f, scaleY = 0.0f, scaleZ = 0.0f;

    if (std::abs(position.x) > invisibleWidth) {
        float scaleFactor = (std::abs(position.x) - invisibleWidth) / (arenaWidth - invisibleWidth);
        scaleX = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.y) > invisibleHeight) {
        float scaleFactor = (std::abs(position.y) - invisibleHeight) / (arenaHeight - invisibleHeight);
        scaleY = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.z) > invisibleDepth) {
        float scaleFactor = (std::abs(position.z) - invisibleDepth) / (arenaDepth - invisibleDepth);
        scaleZ = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
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
        boidApplyForce(nudgeForce);
    }
}


void Boid::boidFleeFromPredator() {
    //std::cout << boidScared << std::endl;                                                 //debug
    if (!boidScared) {
        return;
    }

    float closestDistance = std::numeric_limits<float>::max();
    threepp::Vector3 closestPredatorPosition;
    float scaredDampener = std::pow(2.0f, fearFactor - 7) / speedForceRandomDampener;

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

    //std::cout << fleeForce << std::endl;                                                  //debug
    //std::cout << fearFactor << std::endl;                                                 //debug

    boidApplyForce(fleeForce);
}

void Boid::boidCalculateFearFactor(){
    boidScared = false;

    if (pack1.packGetPredators().empty()) {
        fearFactor = 0;
        //std::cout << "no predators seen" << std::endl;                                  //debug
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

const threepp::Vector3& Boid::boidGetPosition() const {                  // Access methods for use in main if needed
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
        //std::cout << "out of bounds" << std::endl;                              //debug
        return true;
        }

    //std::cout << "not out of bounds" << std::endl;                              //debug
    return false;
}


bool Boid::boidGetBoidScaredCheck() const {

    //std::cout << fearFactor << std::endl;                                       //debug

    return boidScared;
}




