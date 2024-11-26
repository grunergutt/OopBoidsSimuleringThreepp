#include "boid.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "utilityfunctions.hpp"
#include <iostream>

float borderInvisiblePercentage = 0.85;                                 // between 0 and 1
int speedForceRandomDampener = 100;

Boid::Boid(int identifier, int sightRangeInitializer, bool outOfBoundsStatus,  bool boidScaredStatus,
           float maxSpeedInitializer, float maxForceInitializer,
           float randomFactorInitializer)
:   position{threepp::Vector3{
         getRandomFloat(-arena.getArenaWidth() / 2, arena.getArenaWidth() / 2),
         getRandomFloat(-arena.getArenaHeight() / 2, arena.getArenaHeight() / 2),
         getRandomFloat(-arena.getArenaDepth() / 2, arena.getArenaDepth() / 2)}},
    velocity{threepp::Vector3{0, 0, 0}},
    acceleration{threepp::Vector3{0, 0, 0}},
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
    boidConstrainToPhysicalBorders();
    boidApplyRandomForce();
    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
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

        threepp::Vector3 origin(0.0f, 0.0f, 0.0f);
        threepp::Vector3 directionToOrigin = origin - position;

        if (directionToOrigin.length() > 0) {
            directionToOrigin.normalize();
            threepp::Vector3 nudgeForce = directionToOrigin * nudgeStrength;

            boidApplyForce(nudgeForce);
        }
}


void Boid::boidFleeFromPredator() {
    boidCalculateFearFactor();
    boidScared = false;

    if (fearFactor == 0) {
        return;
    }

    if (fearFactor > 4.5) {
        boidScared = true;
    }

    float closestDistance = std::numeric_limits<float>::max();
    threepp::Vector3 closestPredatorPosition;
    float scaredDampener = std::pow(0.5/speedForceRandomDampener, 10 - fearFactor);

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

        fleeDirection = threepp::Vector3(1.0f, 0.0f, 0.0f); // Arbitrary direction
    }
    fleeDirection.normalize();

    threepp::Vector3 fleeForce = (fleeDirection * scaredDampener);

    boidApplyForce(fleeForce);
}

void Boid::boidCalculateFearFactor(){
    if (pack1.packGetPredators().empty()) {
        fearFactor = 0;
        std::cout << "no predators seen" << std::endl;                                  //debug
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

bool Boid::boidGetBoidOutOfBoundsCheck(Boid* boid) const {

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

bool Boid::boidGetBoidScaredCheck() const {

    std::cout << fearFactor << std::endl;                     //debug

    if (fearFactor > 4.5) {
        return true;
    }

    return false;
}




