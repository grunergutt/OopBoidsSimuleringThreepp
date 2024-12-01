#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "threepp/threepp.hpp"
#include "arena.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include "pack.hpp"
#include "predator.hpp"
#include "threeppfunctions.hpp"
#include "utilityfunctions.hpp"

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



