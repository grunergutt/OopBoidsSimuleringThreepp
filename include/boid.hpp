#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"

class Pack;

class Boid {

private:
    threepp::Vector3 position;                                      // Position of boid
    threepp::Vector3 velocity;                                      // Velocity of boid
    threepp::Vector3 acceleration;                                  // Acceleration of boid
    int fearFactor;                                                 // How scared a boid is
    int sightRange;                                                 // how far away it spots a predator
    int boidIdentifier;                                             // id tag for boid so getBoidById works
    bool boidOutOfBounds;                                           // boolean status for if boid should turn back to arena
    bool boidScared;                                                // boolean status for if boid fearfactor is above a certain level
    float maxSpeed;                                                 // Maximum speed, will create slider for this
    float maxForce;                                                 // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;                                        // how much the boids move randomly, Create slidere here
    float dampingFactor;                                            // Damping factor for smoother movement so it is less sparatic.

public:

    Boid();
    Boid(int identifier, int sightRangeInitializer = 35,            // prepares certain default initializer values for boid initialization in boid.cpp
         bool outOfBoundsStatus = false,
         bool boidScaredStatus = false,
         float maxSpeedInitializer = 20,
         float maxForceInitializer = 4,
         float randomFactorInitializer = 0.4);

                                                                    // Method declarations
    void boidApplyRandomForce();                                    // Applies a random froce to boid based on randomForceFactor
    void boidApplyForce(const threepp::Vector3& force);             // aplies general forces to boid
    void boidUpdateVelocity();                                      // updates velocity with acceleration
    void boidUpdatePosition();                                      // updates position using velocity
    void boidUpdateBoid();                                          // chains different updates and checks to simulate boid movement.
    void boidNudgeBoidAwayFromBorder(float nudgeForce);             // based on boidOutOfBounds pushes boid towards origo
    void boidFleeFromPredator();                                    // based on fearFactor and boidScared makes boid flee from predator
    void boidCalculateFearFactor();                                 // calculates fearfactor based on predator proximity

    [[nodiscard]] const threepp::Vector3& boidGetPosition() const;      // "Getters"
    [[nodiscard]] const threepp::Vector3& boidGetVelocity() const;      // marked nodiscard to avoid bugs if return value is unused
    [[nodiscard]] const threepp::Vector3& boidGetAcceleration() const;
    [[nodiscard]] int boidGetBoidIdentifier() const;
    [[nodiscard]] bool boidGetBoidOutOfBoundsCheck() const;
    [[nodiscard]] bool boidGetBoidScaredCheck() const;
    float boidGetMaxSpeed();
    float boidGetMaxForce();
    float boidGetRandomForce();

    void boidSetMaxSpeed(float setMaxSpeed);                        // setters for dynamic change in animation loop
    void boidSetMaxForce(float setMaxForce);
    void boidSetRandomForce(float setRandomForceFactor);

    void boidSetPosition(const threepp::Vector3& setPosition){position = setPosition;};
    void boidSetVelocity(const threepp::Vector3& setVelocity){velocity = setVelocity;};
    void boidSetAcceleration(const threepp::Vector3& setAcceleration){acceleration = setAcceleration;};

};

#endif // BOID_HPP
