#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"
#include <random>

class Arena;

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
    void boidConstrainToBorders();

    [[nodiscard]] const threepp::Vector3& boidGetPosition() const;       // "Getters"
    [[nodiscard]] const threepp::Vector3& boidGetVelocity() const;       // marked nodiscard to avoid bugs if return value is unused
    [[nodiscard]] const threepp::Vector3& boidGetAcceleration() const;

};

#endif // BOID_HPP
