#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include "threepp/threepp.hpp"
#include "arena.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "threeppfunctions.hpp"
#include "utilityfunctions.hpp"
#include <iostream>

TEST_CASE("Arena returns the correct cell indency") {
    Boid boid1(1, 35, false, false, 20, 4, 0.4);
    Arena arena(50, 50, 50, 5);
    boid1.boidSetPosition({0, 25, 50});

    arena.addBoid(&boid1);

    auto [x, y, z] = arena.getCellIndices(boid1.boidGetPosition());

    CHECK(x == 0);
    CHECK(y == 5);
    CHECK(z == 9);
}

TEST_CASE("Arena adds boid to boidgrid") {

    Predator predator1(1, 270, false, false, 20, 4, 0.4);
    Arena arena(5, 5, 5, 5);
    predator1.predatorSetPosition({0, 0, 0});

    arena.addPredator(&predator1);

    auto predatorGrid = arena.getPredatorGrid();

    auto [x, y, z] = arena.getCellIndices(predator1.predatorGetPosition());

    CHECK(predatorGrid[x][y][z].size() == 1);
    CHECK(predatorGrid[x][y][z][0] == &predator1);
}

TEST_CASE("Arena adds predator to predatorGrid") {
    Boid boid1(1, 35, false, false, 20, 4, 0.4);
    Arena arena(5, 5, 5, 5);
    boid1.boidSetPosition({0, 0, 0});

    arena.addBoid(&boid1);

    auto boidGrid = arena.getBoidGrid();

    auto [x, y, z] = arena.getCellIndices(boid1.boidGetPosition());

    CHECK(boidGrid[x][y][z].size() == 1);
    CHECK(boidGrid[x][y][z][0] == &boid1);
}

TEST_CASE("Arena clears boidGrid when called") {
    Boid boid1(1, 35, false, false, 20, 4, 0.4);
    Arena arena(5, 5, 5, 5);
    boid1.boidSetPosition({0, 0, 0});

    arena.addBoid(&boid1);

    auto [x, y, z] = arena.getCellIndices(boid1.boidGetPosition());

    arena.clearGrid();
    auto boidGrid = arena.getBoidGrid();
    CHECK(boidGrid[x][y][z].size() == 0);
}


TEST_CASE("Arena returns the right neigbors") {
    Boid boid1(1, 35, false, false, 20, 4, 0.4);
    Boid boid2(2, 35, false, false, 20, 4, 0.4);
    Arena arena(100, 100, 100, 5);

    boid1.boidSetPosition({30, 30, 30});
    boid2.boidSetPosition({5, 5, 5});

    arena.addBoid(&boid1);
    arena.addBoid(&boid2);

    std::vector<const Boid*> neighbors1 = arena.getNeighboringBoids(boid1, 30);
    std::vector<const Boid*> neighbors2 = arena.getNeighboringBoids(boid2, 2);

    CHECK(neighbors1.size() == 1);
    CHECK(neighbors1[0] == &boid2);

    CHECK(neighbors2.size() == 0);
}

TEST_CASE("Boid position changes when updated"){
    threepp::Vector3 initialPosition(0.0f, 0.0f, 0.0f);
    threepp::Vector3 initialVelocity(1.0f, 0.0f, 0.0f);
    threepp::Vector3 expectedPosition(1.0f, 0.0f, 0.0f);

    Boid boid1(1,35, false, false, 20, 4, 0.4);

    boid1.boidSetPosition(initialPosition);
    boid1.boidSetVelocity(initialVelocity);
    boid1.boidUpdatePosition();
    CHECK(boid1.boidGetPosition() == expectedPosition);
}

TEST_CASE("Boid velocity changes when updated"){
    threepp::Vector3 initialVelocity(0.0f, 0.0f, 0.0f);
    threepp::Vector3 initialAcceleration(0.2f, 0.0f, 0.0f);
    threepp::Vector3 expectedVelocity(0.2f, 0.0f, 0.0f);

    Boid boid1(1,35, false, false, 20, 4, 0.4);

    boid1.boidSetVelocity(initialVelocity);
    boid1.boidSetAcceleration(initialAcceleration);
    boid1.boidUpdateVelocity();
    CHECK(boid1.boidGetVelocity() == expectedVelocity);
}

TEST_CASE("Flock can store boid instances") {
    Flock flock1;

    Boid boid1(0,35, false, false, 20, 4, 0.4);
    Boid boid2(1,35, false, false, 20, 4, 0.4);

    flock1.flockAddBoid(std::make_unique<Boid>(boid1));
    flock1.flockAddBoid(std::make_unique<Boid>(boid2));

    CHECK(flock1.flockGetNumBoids() == 2);

}

TEST_CASE("Flock can get boid by index") {
    Flock flock1;
    flock1.flockAddBoid(std::make_unique<Boid>(0));
    flock1.flockAddBoid(std::make_unique<Boid>(1));

    CHECK(&flock1.getBoidByIndex(0) != &flock1.getBoidByIndex(1));
}

TEST_CASE("Flock can dynamicly change key variables") {
    Flock flock1;
    flock1.flockAddBoid(std::make_unique<Boid>(0));
    flock1.flockAddBoid(std::make_unique<Boid>(1));

    flock1.setAlignmentRadius(1);
    CHECK(flock1.getAlignmentRadius() == 1);

    flock1.setAlignmentRadius(2);
    CHECK(flock1.getAlignmentRadius() == 2);

}

TEST_CASE("Flock iterates over boids and changes Pos, Vel and Acc") {
    int numberOfBoidsFlock1 = 10;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        flock1.flockAddBoid(std::make_unique<Boid>(i));
    }

    Arena arena(5, 5, 5, 1);

    for (auto &boid : flock1.getBoids()) {
        threepp::Vector3 boidPosition = boid->boidGetPosition();
        threepp::Vector3 boidVelocity = boid->boidGetVelocity();
        threepp::Vector3 boidAcceleration = boid->boidGetAcceleration();
        flock1.flockUpdateFlock();
        CHECK(boid->boidGetPosition() != boidPosition);
        CHECK(boid->boidGetVelocity() != boidVelocity);
        CHECK(boid->boidGetAcceleration() != boidAcceleration);
    }
}

TEST_CASE("Predator calculates meaningfull attack point") {
    Arena arena(20, 20, 20, 5);

    int numberOfBoidsFlock1 = 10;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        flock1.flockAddBoid(std::make_unique<Boid>(i));
    }

    threepp::Vector3 boidCentreOfMass;
    for (const auto& boid : flock1.getBoids()) {
        boid->boidSetPosition({10.0f, 4.0f, 7.5f});
        boidCentreOfMass.x += boid->boidGetPosition().x;
        boidCentreOfMass.y += boid->boidGetPosition().y;
        boidCentreOfMass.z += boid->boidGetPosition().z;
    }
    boidCentreOfMass.multiplyScalar(1.0f / numberOfBoidsFlock1); //this is what predator does just with "boidsinsight"

    std::vector<Flock*> flocks = {&flock1};

    Predator predator(1, 360, false, false, 20, 4, 0.4);
    predator.predatorSetPosition({0, 0, 0});

    CHECK(predator.predatorCalculateAttackPoint(flocks).x == Catch::Approx(boidCentreOfMass.x).epsilon(0.1));
    CHECK(predator.predatorCalculateAttackPoint(flocks).y == Catch::Approx(boidCentreOfMass.y).epsilon(0.1));
    CHECK(predator.predatorCalculateAttackPoint(flocks).z == Catch::Approx(boidCentreOfMass.z).epsilon(0.1));
}

TEST_CASE("Pack can hold and return instances of predator") {
    Pack pack1;
    Predator pred1(0);
    Predator pred2(1);

    pack1.packAddPredator(std::make_unique<Predator>(pred1));
    pack1.packAddPredator(std::make_unique<Predator>(pred2));

    CHECK(&pack1.packGetPredatorByIndex(1) != &pack1.packGetPredatorByIndex(0));
    CHECK(pack1.packGetNumPredators() == 2);
}

TEST_CASE("Pack calculates repulsionforce for predators") {
    Pack pack1;
    Predator pred1(0);
    pred1.predatorSetPosition({1, 1, 1});
    Predator pred2(1);
    pred2.predatorSetPosition({2, 2, 2});

    pack1.packAddPredator(std::make_unique<Predator>(pred1));
    pack1.packAddPredator(std::make_unique<Predator>(pred2));

    arena.clearPredatorGrid();
    for (const auto& predator : pack1.packGetPredators()) {
        arena.addPredator(predator.get());
    }

    threepp::Vector3 repulsionForce;
    repulsionForce = pack1.packCalculateSeparation(pred1);

    CHECK(repulsionForce != threepp::Vector3(0, 0, 0));
}

TEST_CASE("Threeppfunctions createConeMeshForObject returns mesh") {
    Boid boid(0);
    std::shared_ptr<threepp::Mesh> boidCone = createConeMeshForObject(boid.boidGetPosition(),
        threepp::Color::cyan,
        5);

    auto meshCheck = std::dynamic_pointer_cast<threepp::Mesh>(boidCone);
    CHECK(boidCone != nullptr);
}

TEST_CASE("Threeppfunctions createAnimationGroupForFlock returns valid group") {
    Boid boid0(0);
    Flock flock1;
    flock1.flockAddBoid(std::make_unique<Boid>(boid0));

    std::vector<std::shared_ptr<threepp::Mesh>> boidCones1;

    std::shared_ptr<threepp::Group> flockAnimationGroup = createAnimationGroupForFlock(flock1,
        threepp::Color::cyan,
        boidCones1, 1);

    auto meshCheck = std::dynamic_pointer_cast<threepp::Group>(flockAnimationGroup);
    CHECK(flockAnimationGroup != nullptr);
}

TEST_CASE("Threeppfunctions createAnimationGroupForPack returns valid group") {
    Predator predator0(0);
    Pack pack1;
    pack1.packAddPredator(std::make_unique<Predator>(predator0));

    std::vector<std::shared_ptr<threepp::Mesh>> predatorCones1;

    std::shared_ptr<threepp::Group> packAnimationGroup = createAnimationGroupForPack(pack1,
        threepp::Color::cyan,
        predatorCones1, 1);

    auto meshCheck = std::dynamic_pointer_cast<threepp::Group>(packAnimationGroup);
    CHECK(packAnimationGroup != nullptr);
}


TEST_CASE("utilityfunctions getRandomFloat return random floating point number") {
    float randomFloat = 11.0f;
    randomFloat = getRandomFloat(0, 10);

    bool withinBounds = false;
    if (0 < randomFloat < 10) {
        withinBounds = true;
    }

    CHECK(randomFloat != 11);
    CHECK(withinBounds);
}