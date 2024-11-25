#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include <threepp/threepp.hpp>
#include "arena.hpp"
#include "utilityfunctions.hpp"
#include <memory>

class Predator {
    private:
    std::vector<std::unique_ptr<Predator>> predators;

        threepp::Vector3 position;
        threepp::Vector3 velocity;
        threepp::Vector3 acceleration;
        int identifier;
        int agression;
        bool attacking;
        float speed;
        int sightAngle;
        int sightDistance;

    public:

    Predator(int predatorIdentifier, bool attackingStatus = false, float speed = 0.25, int fieldOfView = 180)
    : position(threepp::Vector3(0, 0, 0)),
      velocity(threepp::Vector3(0, 0, 0)),
      acceleration(threepp::Vector3(0, 0, 0)),
      identifier(predatorIdentifier),
      agression(0),
      attacking(attackingStatus),
      speed(speed),
      sightAngle(fieldOfView),
      sightDistance(0)
    {
        position = threepp::Vector3(
            getRandomFloat(-arena.getArenaWidth() / 2, arena.getArenaWidth() / 2),
            getRandomFloat(-arena.getArenaDepth() / 2, arena.getArenaDepth() / 2),
            getRandomFloat(-arena.getArenaHeight() / 2, arena.getArenaHeight() / 2));
        sightDistance = arena.getArenaDepth() / 3;
    }

        void predatorSpawnPredator();
        void predatorKillPredator();

        void predatorCalculateAttackPoint();
        void predatorAttack();
        void predatorUpdatePredator();
        void predatorConstrainToPhysicalBorder();

        const threepp::Vector3& predatorGetPosition() const;

        const int predatorCalculateSightAngle() const;

        void setPredatorSightDistance(int distance){sightDistance = distance;};
        void setPredatorAgression(int agressionLevel){agression = agressionLevel;};


    };




#endif //PREDATOR_HPP
