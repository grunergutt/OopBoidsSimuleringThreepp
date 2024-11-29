#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include "threepp/threepp.hpp"
#include "arena.hpp"

class Boid;
class Flock;

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
    int sightRange;
    bool predatorOutOfBounds;
    float maxSpeed;                                                  // Maximum speed, will create slider for this
    float maxForce;                                                  // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;
    float dampingFactor;
    int cooldownFrames;

    public:

    Predator();
  Predator(int predatorIdentifier,
        int fieldOfView = 180,
        bool outOfBoundsStatus = false,
        bool attackingStatus = false,
        float maxSpeedInitializer = 50.0f,
        float maxForceInitializer = 10.0f,
        float randomFactorInitializer = 0.5f);

    threepp::Vector3 predatorCalculateAttackPoint(const std::vector<Flock*>& flocks);
    void predatorAttackPoint(const threepp::Vector3& target);

    void predatorApplyRandomForce();                                     // Method declarations
    void predatorApplyForce(const threepp::Vector3& force);              // this method will add forces calculated from flock class
    void predatorUpdateVelocity();
    void predatorUpdatePosition();
    void predatorUpdatePredator(const std::vector<Flock*>& flocks);
    void predatorNudgePredatorAwayFromBorder(float nudgeForce);
    void predatorNudgeAwayFromPredators(float nudgeForce);


    const threepp::Vector3& predatorGetPosition() const;
    const threepp::Vector3& predatorGetVelocity() const;
    const threepp::Vector3& predatorGetAcceleration() const;
    const bool predatorGetOutOfBoundsCheck() const;

    const int predatorCalculateSightAngle() const;

    void setPredatorSightDistance(int distance){sightRange = distance;};
    void setPredatorAgression(int agressionLevel){aggression = agressionLevel;};


    };




#endif //PREDATOR_HPP