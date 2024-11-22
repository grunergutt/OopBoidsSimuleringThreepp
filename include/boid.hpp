#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"
#include "arena.hpp"
#include "threeppfunctions.hpp"

class Arena;



class Boid {

private:
    threepp::Vector3 position;                                       // Position of boid
    threepp::Vector3 velocity;                                       // Velocity of boid
    threepp::Vector3 acceleration;                                   // Acceleration of boid
    float fearFactor;
    int boidIdentifier;
    bool boidOutOfBounds;
    float maxSpeed;                                                  // Maximum speed, will create slider for this
    float maxForce;                                                  // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;                                         // how much the boids move randomly, Create slidere here
    float dampingFactor;                                             // Damping factor for smoother movement so it is less sparatic.

public:
    Boid(int identifier, bool outOfBoundsStatus = false, float maxSpeedInitializer = 0.5, float maxForceInitializer = 0.05, float randomFactorInitializer = 0.005)     //make it so you can use default
    : position(threepp::Vector3(
        getRandomFloat(- arena.getArenaWidth()/2, arena.getArenaWidth()/2),
        getRandomFloat(- arena.getArenaDepth()/2, arena.getArenaDepth()/2),
        getRandomFloat(- arena.getArenaHeight()/2, arena.getArenaHeight()/2))),                                                                  //or your own values when creating a boid
      velocity(threepp::Vector3(0, 0, 0)),
      acceleration(threepp::Vector3(0, 0, 0)),
      fearFactor(0),
      boidIdentifier(identifier),
      boidOutOfBounds(outOfBoundsStatus),
      maxSpeed(maxSpeedInitializer),
      maxForce(maxForceInitializer),
      randomForceFactor(randomFactorInitializer),
      dampingFactor(0.9f) {}


    void boidApplyRandomForce();                                     // Method declarations
    void boidApplyForce(const threepp::Vector3& force);              // this method will add forces calculated from flock class
    void boidUpdateVelocity();
    void boidUpdatePosition();
    void boidUpdateBoid();
    void boidConstrainToPhysicalBorders();
    void boidNudgeBoidAwayFromBorder();
    void boidFleeFromPredator();

    [[nodiscard]] const threepp::Vector3& boidGetPosition() const;       // "Getters"
    [[nodiscard]] const threepp::Vector3& boidGetVelocity() const;       // marked nodiscard to avoid bugs if return value is unused
    [[nodiscard]] const threepp::Vector3& boidGetAcceleration() const;
    [[nodiscard]] int boidGetBoidIdentifier() const;
    [[nodiscard]] bool boidGetBoidOutOfBoundsCheck(Boid* boid) const;
    [[nodiscard]] int boidCalculateFearFactor() const;

    void boidSetMaxSpeed(float setMaxSpeed) {maxSpeed = setMaxSpeed;}
    void boidSetMaxForce(float setMaxForce) {maxForce = setMaxForce;}
    void boidSetRandomForce(float setRandomForceFactor) {randomForceFactor = setRandomForceFactor;}
    void boidSetDampingFactor(float setDampingFactor) {dampingFactor = setDampingFactor;}



};

#endif // BOID_HPP
