#include "boid.hpp"

float borderInvisiblePercentage = 0.85;                                 // between 0 and 1

void Boid::boidApplyRandomForce() {
    threepp::Vector3 randomForce(                                    // Generate a random force with each component between -0.2 and 0.2
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor),
        getRandomFloat(-randomForceFactor, randomForceFactor)
    );

    acceleration.add(randomForce);
    if (acceleration.length() > maxForce) {                           // Limit the random force to maxForce to keep it within acceptable range
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }

}

void Boid::boidApplyForce(const threepp::Vector3& flockForce) {     // Function adds flocking forces calculated in flock class

    acceleration.add(flockForce);
    if (acceleration.length() > maxForce) {                         // Same as random force function.
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Boid::boidUpdateVelocity() {

    velocity.add(acceleration);                                      // Update velocity based on acceleration

    if (velocity.length() > maxSpeed) {                              // Limit speed to maxSpeed
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }

    velocity.multiplyScalar(dampingFactor);
}

void Boid::boidUpdatePosition() {
    position.add(velocity);                                          // Update position based on velocity

}

void Boid::boidUpdateBoid() {
    boidConstrainToPhysicalBorders();
    //boidNudgeBoidAwayFromBorder();
    boidApplyRandomForce();
    boidUpdateVelocity();                                            // Chain update steps for animation frame by frame
    boidUpdatePosition();
}

void Boid::boidConstrainToPhysicalBorders() {
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

void Boid::boidNudgeBoidAwayFromBorder() {
    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;

    threepp::Vector3 origo(0, 0, 0);
    threepp::Vector3 repulsionForce(0, 0, 0);

    // X-Axis repulsion
    if (position.x >= width / 2 ) {
        repulsionForce.x = -1.0f * (width / 2 - position.x) / width; // Push left
    } else if (position.x <= -width / 2 ) {
        repulsionForce.x = 1.0f * (-width / 2 - position.x) / width; // Push right
    }

    // Y-Axis repulsion
    if (position.y >= height / 2 ) {
        repulsionForce.y = -1.0f * (height / 2 - position.y) / height; // Push down
    } else if (position.y <= -height / 2 ) {
        repulsionForce.y = 1.0f * (-height / 2 - position.y) / height; // Push up
    }

    // Z-Axis repulsion
    if (position.z >= depth / 2) {
        repulsionForce.z = -1.0f * (depth / 2 - position.z) / depth; // Push back
    } else if (position.z <= -depth / 2) {
        repulsionForce.z = 1.0f * (-depth / 2 - position.z) / depth; // Push forward
    }

    // Apply repulsion force to acceleration
    acceleration += repulsionForce;
}

void Boid::boidFleeFromPredator() {

}

const threepp::Vector3& Boid::boidGetPosition() const {                  // Access methods for use in main if needed
    return position;
}

const threepp::Vector3& Boid::boidGetVelocity() const {
    return velocity;
}

const threepp::Vector3& Boid::boidGetAcceleration() const {
    return acceleration;
}

int Boid::boidGetBoidIdentifier() const {
    return boidIdentifier;
}

bool Boid::boidGetBoidOutOfBoundsCheck(Boid* boid) const {

    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;


    const threepp::Vector3& position = boid->boidGetPosition();


    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {
        return true;
        }

    return false;
}

int Boid::boidCalculateFearFactor() const {
    threepp::Vector3 Distance(0,0, 0);
    int fearAmount = 0;
    std::vector<std::unique_ptr<Predator>> predators = predator.predatorGetPredators;

    for (auto& predator : predators)
    Distance = position -

    boidSetFearFactor(fearAmount)
    return 0;
}



