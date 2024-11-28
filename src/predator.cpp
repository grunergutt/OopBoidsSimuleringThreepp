#include  "predator.hpp"
#include <iostream>

extern int speedForceRandomDampener;

Predator::Predator(int predatorIdentifier,
                   int fieldOfView,
                   bool outOfBoundsStatus,
                   bool attackingStatus,
                   float maxSpeedInitializer,
                   float maxForceInitializer,
                   float randomFactorInitializer)
    : position{threepp::Vector3{
        getRandomFloat(-arena.getArenaWidth() / 2, arena.getArenaWidth() / 2),
        getRandomFloat(-arena.getArenaHeight() / 2, arena.getArenaHeight() / 2),
        getRandomFloat(-arena.getArenaDepth() / 2, arena.getArenaDepth() / 2)}},
    velocity{threepp::Vector3{0, 0, 0}},
    acceleration{threepp::Vector3{0.1, 0.1, 0.1}},
    identifier(predatorIdentifier),
    aggression(0),
    sightAngle(fieldOfView),
    sightDistance(arena.getArenaDepth() / 3),
    attacking(attackingStatus),
    predatorOutOfBounds(outOfBoundsStatus),
    maxSpeed(maxSpeedInitializer / static_cast<float>(speedForceRandomDampener)),
    maxForce(maxForceInitializer / static_cast<float>(speedForceRandomDampener)),
    randomForceFactor(randomFactorInitializer / static_cast<float>(speedForceRandomDampener)),
    dampingFactor(0.9f) {}

void Predator::predatorSpawnPredator() {

}
void predatorKillPredator();

void predatorCalculateAttackPoint();
void predatorAttack();

void Predator::predatorApplyRandomForce(){

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
    //std::cout << "Random: " << randomForce << " Acc: " << acceleration << std::endl;  //debug
}

void Predator::predatorApplyForce(const threepp::Vector3& force) {     // Function adds flocking forces calculated in flock class

    acceleration.add(force);
    if (acceleration.length() > maxForce) {                                 // Same as random force function.
        acceleration.normalize();
        acceleration.multiplyScalar(maxForce);
    }
}

void Predator::predatorUpdateVelocity() {

    velocity.add(acceleration);                                      // Update velocity based on acceleration

    if (velocity.length() > maxSpeed) {                              // Limit speed to maxSpeed
        velocity.normalize();
        velocity.multiplyScalar(maxSpeed);
    }

    velocity.multiplyScalar(dampingFactor);
}

void Predator::predatorUpdatePosition() {
    position.add(velocity);                                          // Update position based on velocity

}

void Predator::predatorUpdatePredator() {

    if (predatorGetOutOfBoundsCheck()) {
        float nudgeForce = maxForce / 10.0f;
        predatorNudgePredatorAwayFromBorder(nudgeForce);
    }

    if (!predatorGetOutOfBoundsCheck()) {
        predatorApplyRandomForce();
    }

    predatorUpdateVelocity();                                            // Chain update steps for animation frame by frame
    predatorUpdatePosition();
    //std::cout  << ", Predator maxSpeed: " << maxSpeed << "\n" <<  ", Predator maxForce: " << maxForce << std::endl;           //debug
}

void Predator::predatorNudgePredatorAwayFromBorder(float nudgeStrength) {
    extern float borderInvisiblePercentage;

    float arenaWidth = arena.getArenaWidth() / 2;
    float arenaHeight = arena.getArenaHeight() / 2;
    float arenaDepth = arena.getArenaDepth() / 2;
    float invisibleWidth = arenaWidth * borderInvisiblePercentage;
    float invisibleHeight = arenaHeight * borderInvisiblePercentage;
    float invisibleDepth = arenaDepth * borderInvisiblePercentage;

    threepp::Vector3 nudgePoint(getRandomFloat(-invisibleWidth, invisibleWidth), getRandomFloat(-invisibleHeight, invisibleHeight), getRandomFloat(-invisibleDepth, invisibleDepth));
    threepp::Vector3 directionToOrigin = nudgePoint - position;

    float scaleX = 0.0f, scaleY = 0.0f, scaleZ = 0.0f;

    if (std::abs(position.x) > invisibleWidth) {
        float scaleFactor = (std::abs(position.x) - invisibleWidth) / (arenaWidth - invisibleWidth);
        scaleX = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.y) > invisibleHeight) {
        float scaleFactor = (std::abs(position.y) - invisibleHeight) / (arenaHeight - invisibleHeight);
        scaleY = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
    }

    if (std::abs(position.z) > invisibleDepth) {
        float scaleFactor = (std::abs(position.z) - invisibleDepth) / (arenaDepth - invisibleDepth);
        scaleZ = std::pow(scaleFactor, 2.0f); // Quadratic scaling for stronger growth near hard border
    }

    // Calculate nudge force if outside invisible border
    if (std::abs(position.x) > invisibleWidth ||
        std::abs(position.y) > invisibleHeight ||
        std::abs(position.z) > invisibleDepth) {

        directionToOrigin.normalize();

        threepp::Vector3 nudgeForce = directionToOrigin * nudgeStrength;
        nudgeForce.x *= scaleX; // Apply scaling in x-direction
        nudgeForce.y *= scaleY; // Apply scaling in y-direction
        nudgeForce.z *= scaleZ; // Apply scaling in z-direction


        // Apply only the nudge force
        predatorApplyForce(nudgeForce);
    }
}

const threepp::Vector3& Predator::predatorGetPosition() const{
    return position;
}

const threepp::Vector3& Predator::predatorGetVelocity() const {
    return velocity;
}

const threepp::Vector3& Predator::predatorGetAcceleration() const {
    return acceleration;
}

const bool Predator::predatorGetOutOfBoundsCheck() const {

    extern float borderInvisiblePercentage;
    float width = arena.getArenaWidth() * borderInvisiblePercentage;
    float height = arena.getArenaHeight() * borderInvisiblePercentage;
    float depth = arena.getArenaDepth() * borderInvisiblePercentage;


    const threepp::Vector3& position = predatorGetPosition();


    if (position.x > width / 2 || position.x < -width / 2 ||
        position.y > height / 2 || position.y < -height / 2 ||
        position.z > depth / 2 || position.z < -depth / 2) {
        //std::cout << "out of bounds" << std::endl;                              //debug
        return true;
        }

    //std::cout << "not out of bounds" << std::endl;                              //debug
    return false;
}

const int Predator::predatorCalculateSightAngle() const {
    return 0;
}