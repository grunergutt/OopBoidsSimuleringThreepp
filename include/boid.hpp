#ifndef BOID_HPP
#define BOID_HPP

#include "threepp/threepp.hpp"
#include <random>

// Hjelpefunksjon for å få en tilfeldig flyttall mellom min og maks
inline float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

class Boid {

private:
    threepp::Vector3 position;       // Posisjon til boid
    threepp::Vector3 velocity;       // Hastighet til boid
    threepp::Vector3 acceleration;   // Akselerasjon til boid
    float maxSpeed;                  // Maks hastighet
    float maxForce;                  // Maks kraft som kan legges til akselerasjon
    float dampingFactor;             // Dempingsfaktor for glattere bevegelse

public:
    // Konstruktør
    Boid() : position(threepp::Vector3(0, 0, 0)),
             velocity(threepp::Vector3(0, 0, 0)),
             acceleration(threepp::Vector3(0, 0, 0)),
             maxSpeed(2.0),
             maxForce(1.0),
             dampingFactor(0.9) {}

    // Metoder
    void applyRandomForce();
    void updateVelocity();
    void updatePosition();
    void boidUpdateBoid();
    void boidConstrainToBorders(float width, float height, float depth);

    // "Getters"
    [[nodiscard]] const threepp::Vector3& getPosition() const;      //nodiscard for å hjelpe med bugfiks om jeg
    [[nodiscard]] const threepp::Vector3& getVelocity() const;      //glemmer å bruke verdien etter funksjonskall
    [[nodiscard]] const threepp::Vector3& getAcceleration() const;
};



void Boid::applyRandomForce() {
    // Generer en tilfeldig kraft med hver komponent mellom -0.2 og 0.2
    threepp::Vector3 randomForce(
        getRandomFloat(-0.2f, 0.2f),
        getRandomFloat(-0.2f, 0.2f),
        getRandomFloat(-0.2f, 0.2f)
    );

    // Begrens den tilfeldige kraften til maxForce for å holde den innenfor akeptable mengder
    if (randomForce.length() > maxForce) {
        randomForce.normalize();
        randomForce.multiplyScalar(maxForce);
    }
    acceleration.add(randomForce);
}

void Boid::updateVelocity() {
    // Oppdater hastigheten basert på akselerasjon
    velocity.add(acceleration);

    // Begrens hastigheten til maxSpeed
    if (velocity.length() > maxSpeed) {
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }
}

void Boid::updatePosition() {
    // Oppdater posisjonen basert på hastigheten
    position.add(velocity);

    // Bruk demping for å glatte ut akselerasjon, for å hindre sparadiske bevegelser som ikke er naturlige
    acceleration.multiplyScalar(dampingFactor);
}

void Boid::boidUpdateBoid() {
    // Kjede oppdateringsstegene for animasjon frame by frame
    updateVelocity();
    updatePosition();
}

void Boid::boidConstrainToBorders(float width, float height, float depth) {

    // Begrens posisjonen til spesifisert bredde, høyde og dybde, delt på to fordi de er begrenset av at de er
    // Basert rundt origo

    if (position.x >= width / 2) {
        position.x = width / 2;
    }
    if (position.x <= -width / 2) {
        position.x = -width / 2;
        velocity.x *= -1;
    }

    if (position.y >= height) {
        position.y = height;
        velocity.y *= -1;
    }
    if (position.y <= 0) {
        position.y = 0;
        velocity.y *= -1;
    }

    if (position.z >= depth / 2) {
        position.z = depth / 2;
        velocity.z *= -1;
    }
    if (position.z <= -depth / 2) {
        position.z = -depth / 2;
        velocity.z *= -1;
    }
}

// Tilgangsmetodene fulført
const threepp::Vector3& Boid::getPosition() const {
    return position;
}

const threepp::Vector3& Boid::getVelocity() const {
    return velocity;
}

const threepp::Vector3& Boid::getAcceleration() const {
    return acceleration;
}

#endif // BOID_HPP
