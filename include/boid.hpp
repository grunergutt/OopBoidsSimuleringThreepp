#ifndef BOID_HPP
#define BOID_HPP
#include <threepp/math/Vector2.hpp>  /
#include <vector>

class Boid {
private:
    threepp::Vector2 position;
    threepp::Vector2 velocity;
    threepp::Vector2 acceleration;
    float maxForce;
    float maxSpeed;

public:
    Boid(float x, float y);

	void applyForce(const threepp::Vector2& force);
    void update();
    void edges(float screenWidth, float screenHeight);
    threepp::Vector2 align(const std::vector<Boid>& boids, float perceptionRadius);
};

#endif //BOID_HPP
