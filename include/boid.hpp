#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"

class Pack;

class Boid {

private:
    threepp::Vector3 position;                                       // Position of boid
    threepp::Vector3 velocity;                                       // Velocity of boid
    threepp::Vector3 acceleration;                                   // Acceleration of boid
    int fearFactor;
    int sightRange;
    int boidIdentifier;
    bool boidOutOfBounds;
    bool boidScared;
    float maxSpeed;                                                  // Maximum speed, will create slider for this
    float maxForce;                                                  // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;                                         // how much the boids move randomly, Create slidere here
    float dampingFactor;                                             // Damping factor for smoother movement so it is less sparatic.

public:

    Boid();
    Boid(int identifier, int sightRangeInitializer = 35,
         bool outOfBoundsStatus = false,
         bool boidScaredStatus = false,
         float maxSpeedInitializer = 20,
         float maxForceInitializer = 4,
         float randomFactorInitializer = 0.4);

    void boidApplyRandomForce();                                     // Method declarations
    void boidApplyForce(const threepp::Vector3& force);              // this method will add forces calculated from flock class
    void boidUpdateVelocity();
    void boidUpdatePosition();
    void boidUpdateBoid();
    void boidConstrainToPhysicalBorders();
    void boidNudgeBoidAwayFromBorder(float nudgeForce);
    void boidFleeFromPredator();
    void boidCalculateFearFactor();

    [[nodiscard]] const threepp::Vector3& boidGetPosition() const;       // "Getters"
    [[nodiscard]] const threepp::Vector3& boidGetVelocity() const;       // marked nodiscard to avoid bugs if return value is unused
    [[nodiscard]] const threepp::Vector3& boidGetAcceleration() const;
    [[nodiscard]] int boidGetBoidIdentifier() const;
    [[nodiscard]] bool boidGetBoidOutOfBoundsCheck() const;
    [[nodiscard]] bool boidGetBoidScaredCheck() const;


    void boidSetMaxSpeed(float setMaxSpeed) {maxSpeed = setMaxSpeed;}
    void boidSetMaxForce(float setMaxForce) {maxForce = setMaxForce;}
    void boidSetRandomForce(float setRandomForceFactor) {randomForceFactor = setRandomForceFactor;}
    void boidSetDampingFactor(float setDampingFactor) {dampingFactor = setDampingFactor;}
    void boidSetFearFactor(int setFearFactor) {fearFactor = setFearFactor;}



};

#endif // BOID_HPP
