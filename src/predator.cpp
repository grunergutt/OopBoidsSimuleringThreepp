#include  "predator.hpp"



void predatorSpawnPredator();
void predatorKillPredator();

void predatorCalculateAttackPoint();
void predatorAttack();
void predatorUpdatePredator();

void Predator::predatorConstrainToPhysicalBorder() {
    float width = arena.getArenaWidth();
    float height= arena.getArenaHeight();
    float depth = arena.getArenaDepth();

    if (position.x >= width / 2 || position.x <= -width / 2) {
        acceleration.x *= -1;
        position.x = std::clamp(position.x, -width / 2, width / 2);
    }
    if (position.y >= height / 2 || position.y <= -height / 2) {
        acceleration.y *= -1;
        position.y = std::clamp(position.y, -height / 2, height / 2);
    }
    if (position.z >= depth / 2 || position.z <= -depth / 2 ) {
        acceleration.z *= -1;
        position.z = std::clamp(position.z, -depth / 2, depth / 2);
    }
}

const threepp::Vector3& Predator::predatorGetPosition() const{
    return position;
}

const int Predator::predatorCalculateSightAngle() const {
    return 0;
}