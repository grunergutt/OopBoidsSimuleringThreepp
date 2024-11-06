#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"
#include <random>


//gpt lagd randomfloat funksjon
inline float getRandomFloat(float min, float max) {                             // Function to get a random float between min and max
    static std::random_device rd;                                               // Static to initialize only once
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);    // Uniform distribution between 0 and 1

    return min + dist(mt) * (max - min);                                     // Scale result to desired range
}

class Boid {

private:
    threepp::Vector3 position;                                       // Position of boid
    threepp::Vector3 velocity;                                       // Velocity of boid
    threepp::Vector3 acceleration;                                   // Acceleration of boid
    float maxSpeed;                                                  // Maximum speed, will create slider for this
    float maxForce;                                                  // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;                                         // how much the boids move randomly, Create slidere here
    float dampingFactor;                                             // Damping factor for smoother movement so it is less sparatic.

public:

    Boid(float maxSpeedInitializer = 3, float maxForceInitializer = 2, float randomFactorInitializer = 0.2)     //make it so you can use default
    : position(threepp::Vector3(0, 0, 0)),                                                                //or your own values when creating a boid
      velocity(threepp::Vector3(0, 0, 0)),
      acceleration(threepp::Vector3(0, 0, 0)),
      maxSpeed(maxSpeedInitializer),
      maxForce(maxForceInitializer),
      randomForceFactor(randomFactorInitializer),
      dampingFactor(0.9f) {}

    void boidApplyRandomForce();                                     // Method declarations
    void boidApplyForce(const threepp::Vector3& force);              // this method will add forces calculated from flock class
    void boidUpdateVelocity();
    void boidUpdatePosition();
    void boidUpdateBoid();
    void boidConstrainToBorders(float width, float height, float depth);


    [[nodiscard]] const threepp::Vector3& boidGetPosition() const;       // "Getters"
    [[nodiscard]] const threepp::Vector3& boidGetVelocity() const;       // marked nodiscard to avoid bugs if return value is unused
    [[nodiscard]] const threepp::Vector3& boidGetAcceleration() const;
};

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

    //acceleration.multiplyScalar(dampingFactor);                      // Apply damping to smooth acceleration, to prevent unnatural, erratic motion
}

void Boid::boidUpdateBoid() {
    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
}

void Boid::boidConstrainToBorders(float width, float height, float depth) {
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

#endif // BOID_HPP