#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include <threepp/threepp.hpp>
#include "arena.hpp"
#include "utilityfunctions.hpp"
#include <memory>

class Boid;

class Predator {
    private:
    std::vector<std::unique_ptr<Predator>> predators;

        threepp::Vector3 position;
        threepp::Vector3 velocity;
        threepp::Vector3 acceleration;
        int identifier;
        int aggression;
        bool attacking;
        int sightAngle;
        int sightDistance;
        bool predatorOutOfBounds;
        float maxSpeed;                                                  // Maximum speed, will create slider for this
        float maxForce;                                                  // Maximum force applicable to acceleration, will create slider here too
        float randomForceFactor;
        float dampingFactor;

    public:

    Predator();
  Predator(int predatorIdentifier,
        int fieldOfView = 180,
        bool outOfBoundsStatus = false,
        bool attackingStatus = false,
        float maxSpeedInitializer = 15.0f,
        float maxForceInitializer = 5.0f,
        float randomFactorInitializer = 0.5f);

    void predatorSpawnPredator();
    void predatorKillPredator();

    void predatorCalculateAttackPoint();
    void predatorAttack();

    void predatorApplyRandomForce();                                     // Method declarations
    void predatorApplyForce(const threepp::Vector3& force);              // this method will add forces calculated from flock class
    void predatorUpdateVelocity();
    void predatorUpdatePosition();
    void predatorUpdatePredator();
    void predatorNudgePredatorAwayFromBorder(float nudgeForce);


    const threepp::Vector3& predatorGetPosition() const;
    const threepp::Vector3& predatorGetVelocity() const;
    const threepp::Vector3& predatorGetAcceleration() const;
    const bool predatorGetOutOfBoundsCheck() const;

    const int predatorCalculateSightAngle() const;

    void setPredatorSightDistance(int distance){sightDistance = distance;};
    void setPredatorAgression(int agressionLevel){aggression = agressionLevel;};


    };




#endif //PREDATOR_HPP
