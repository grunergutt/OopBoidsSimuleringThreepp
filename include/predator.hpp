#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include "threepp/threepp.hpp"

class Boid;
class Flock;

class Predator {
    private:

    threepp::Vector3 position;                                        // stores predator position
    threepp::Vector3 velocity;                                        // stores predator velocity
    threepp::Vector3 acceleration;                                    // stores predator acceleration
    int identifier;                                                   // identifier to make predators unique
    bool attacking;                                                   // boolean value if predator is currently attacking
    int sightAngle;                                                   // angle which predators can see boids
    int sightRange;                                                   // range which predators can see boids
    bool predatorOutOfBounds;                                         // boolean value that says if predator is out of bounds
    float maxSpeed;                                                   // Maximum speed, will create slider for this
    float maxForce;                                                   // Maximum force applicable to acceleration, will create slider here too
    float randomForceFactor;                                          // force which predator moves randomly
    float dampingFactor;                                              // the dampingfactor of each animation frame so rpedators arent too fast

    public:

    Predator();
  Predator(int predatorIdentifier,                                    // initializes predators with default values
        int fieldOfView = 270,
        bool outOfBoundsStatus = false,
        bool attackingStatus = false,
        float maxSpeedInitializer = 25.0f,
        float maxForceInitializer = 1.5f,
        float randomFactorInitializer = 0.2f);

    threepp::Vector3 predatorCalculateAttackPoint(                    // lets predator calculate a spesific attack point based on boids
      const std::vector<Flock*>& flocks);
    void predatorAttackPoint(const threepp::Vector3& target);         // makes predator track and attack a spesific point

    void predatorApplyRandomForce();                                  // Method declarations
    void predatorApplyForce(const threepp::Vector3& force);           // this method will add forces calculated from flock class
    void predatorUpdateVelocity();                                    // updates predator with acceleration
    void predatorUpdatePosition();                                    // udates position with velocity
    void predatorUpdatePredator(const std::vector<Flock*>& flocks);   // chains updates to animate predator
    void predatorNudgePredatorAwayFromBorder(float nudgeForce);       // makes predator turn towards origo based on predatoroutofbounds

    const threepp::Vector3& predatorGetPosition() const;              // getters
    const threepp::Vector3& predatorGetVelocity() const;
    const threepp::Vector3& predatorGetAcceleration() const;
    const bool predatorGetOutOfBoundsCheck() const;

    void predatorSetPosition(const threepp::Vector3& position) {      // setter for predators position, its
    this->position = position;                                        // different because animationloop in main
  }                                                                   // removes predators.

  void predatorSetVelocity(const threepp::Vector3& setVelocity){velocity = setVelocity;};
  void predatorSetAcceleration(const threepp::Vector3& setAcceleration){acceleration = setAcceleration;};

  void predatorSetMaxForce(const float& setMaxForce){maxForce = setMaxForce;};


    };




#endif //PREDATOR_HPP