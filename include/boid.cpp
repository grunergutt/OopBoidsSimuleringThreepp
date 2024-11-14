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
    boidConstrainToBorders();
}

void Boid::boidConstrainToBorders() {
    float width = arena.getArenaWidth();
    float height= arena.getArenaHeight();
    float depth = arena.getArenaDepth();

    if (position.x >= width / 2 || position.x <= -width / 2) {
        velocity.x *= -1;
        position.x = std::clamp(position.x, -width / 2, width / 2);
    }
    if (position.y >= height / 2 || position.y <= -height / 2) {
        velocity.y *= -1;
        position.y = std::clamp(position.y, -height / 2, height / 2);
    }
    if (position.z >= depth / 2 || position.z <= -depth / 2) {
        velocity.z *= -1;
        position.z = std::clamp(position.z, -depth / 2, depth / 2);
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