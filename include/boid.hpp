#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"
#include <vector>

class Boid {
public:
    threepp::Vector3 position;  // Current position of the boid
    threepp::Vector3 velocity;  // Current velocity of the boid
    threepp::Vector3 acceleration;  // Acceleration due to forces
    float maxSpeed;  // Maximum speed the boid can reach
    float maxForce;  // Maximum steering force

    // Constructor to initialize the boid with a starting position
    explicit Boid(threepp::Vector3 startPos)
        : position(startPos),
          velocity(threepp::Vector3()),
          acceleration(threepp::Vector3()),
          maxSpeed(0.4f),   // Reduced speed
          maxForce(0.02f)   // Reduced force for slower movement
    {}

    // Apply a force (Vector3) to the acceleration (pass by reference to avoid copying)
    void applyForce(const threepp::Vector3& force) {
        acceleration.add(force);
    }

    // Update the position, velocity, and reset acceleration
    void update() {
        // Update velocity with acceleration
        velocity.add(acceleration);

        // Limit the speed by clamping its length
        if (velocity.length() > maxSpeed) {
            velocity.setLength(maxSpeed);  // Directly set the velocity to the max speed
        }

        // Update position based on the velocity
        position.add(velocity);

        // Reset acceleration to zero using multiplyScalar
        acceleration.multiplyScalar(0);
    }

    // Constrain the boid within the boundaries of the camera
    void constrainToBorders(float minX, float maxX, float minY, float maxY) {
        if (position.x < minX) {
            position.x = minX;
            velocity.x *= -1;  // Reverse direction when hitting the border
        }
        if (position.x > maxX) {
            position.x = maxX;
            velocity.x *= -1;
        }
        if (position.y < minY) {
            position.y = minY;
            velocity.y *= -1;
        }
        if (position.y > maxY) {
            position.y = maxY;
            velocity.y *= -1;
        }
    }
};

#endif //BOID_HPP
