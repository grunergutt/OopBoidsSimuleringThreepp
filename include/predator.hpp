#ifndef PREDATOR_HPP
#define PREDATOR_HPP
#include <threepp/threepp.hpp>


class Predator {
    private:
        threepp::Vector3 position;
        threepp::Vector3 velocity;
        int agression;
        float speed;
        int sightAngle;
        int sightDistance;

    public:
        void predatorSpawnPredator();
        void predatorKillPredator();

        void predatorCalculateAttackPoint();
        void predatorAttack();


    };




#endif //PREDATOR_HPP
