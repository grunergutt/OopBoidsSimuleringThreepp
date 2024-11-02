#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Boid.hpp"
#include <vector>

class Flock {
private:
    std::vector<Boid> boids;         	// Vektor som lagrer alle boidene i flokken
    float separationStrength;        	// Styrke for separasjon
    float alignmentStrength;         	// Styrke for justering
    float cohesionStrength;          	// Styrke for samhold

public:
    // Konstruktør som initialiserer flokk med spesifikke styrker for hver prinsipp
    Flock(int numBoids, float separation, float alignment, float cohesion)
        : separationStrength(separation), alignmentStrength(alignment), cohesionStrength(cohesion) {
        for (int i = 0; i < numBoids; ++i) {
            boids.emplace_back();  		// Legger til nye boider i flokken
        }
    }

    // Metode for å anvende flokkens prinsipper på hver boid
    void applyFlockingPrinciples() {
        for (auto &boid : boids) {
            // Beregn krefter for separasjon, justering, og samhold
            threepp::Vector3 separationForce = calculateSeparation(boid);
            threepp::Vector3 alignmentForce = calculateAlignment(boid);
            threepp::Vector3 cohesionForce = calculateCohesion(boid);

            // Skaler kreftene med deres styrker og legg dem til boidens akselerasjon
            separationForce.multiplyScalar(separationStrength);
            alignmentForce.multiplyScalar(alignmentStrength);
            cohesionForce.multiplyScalar(cohesionStrength);

            boid.getAcceleration().add(separationForce);
            boid.getAcceleration().add(alignmentForce);
            boid.getAcceleration().add(cohesionForce);
        }
    }

    // Oppdatering av flokken, inkludert anvendelse av prinsipper og oppdatering av hver boid
    void updateFlock() {
        applyFlockingPrinciples();  // Bruk flokkprinsipper på hver boid
        for (auto &boid : boids) {
            boid.boidUpdateBoid();  // Oppdater posisjon og hastighet basert på akselerasjon
        }
    }

    // Metoder for beregning av separasjon, justering og samhold (ikke implementert ennå)
    threepp::Vector3 calculateSeparation(Boid &boid);
    threepp::Vector3 calculateAlignment(Boid &boid);
    threepp::Vector3 calculateCohesion(Boid &boid);

    // Getter for å få tilgang til listen av boider i flokken
    const std::vector<Boid>& getBoids() const {
        return boids;
    }
};

#endif // FLOCK_HPP
