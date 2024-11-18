#include "boid.hpp"
#include "arena.hpp"
#include "threepp/threepp.hpp"



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
    boidApplyRandomForce();
    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
    boidConstrainToPhysicalBorders();
    boidConstrainToInvisibleBorders();
}

void Boid::boidSteerTowardsOrigo() {
    threepp::Vector3 origin(0, 0, 0);
    threepp::Vector3 directionToOrigin = origin - position;

    directionToOrigin.normalize();
    float steeringForce = 0.7f;

    directionToOrigin.multiplyScalar(steeringForce);
    acceleration.multiplyScalar(steeringForce);
    velocity.multiplyScalar(0);
    velocity.add(directionToOrigin);
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

void Boid::boidConstrainToInvisibleBorders() {
    float width = arena.getArenaWidth()*0.95;
    float height= arena.getArenaHeight()*0.95;
    float depth = arena.getArenaDepth()*0.95;
    float accelerationDamper = 0.5;

    bool outOfBounds = false;

    if (position.x >= width / 2 || position.x <= -width / 2) {
        acceleration.x *= accelerationDamper;
        position.x = std::clamp(position.x, -width / 2, width / 2);
        outOfBounds = true;
    }
    if (position.y >= height / 2 || position.y <= -height / 2) {
        acceleration.y *= accelerationDamper;
        position.y = std::clamp(position.y, -height / 2, height / 2);
        outOfBounds = true;
    }
    if (position.z >= depth / 2 || position.z <= -depth / 2) {
        acceleration.z *= accelerationDamper;
        position.z = std::clamp(position.z, -depth / 2, depth / 2);
        outOfBounds = true;
    }
    if (outOfBounds) {
        boidSteerTowardsOrigo();
    }
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