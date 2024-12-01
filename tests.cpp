#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Boid.h"
#include "Arena.h"

// Test for Boid class
TEST_CASE("Boid initial position is correct", "[Boid]") {
    Boid boid({1.0f, 2.0f, 3.0f});
    REQUIRE(boid.boidGetPosition() == glm::vec3(1.0f, 2.0f, 3.0f));
}

// Test for Arena class
TEST_CASE("getNeighboringBoids returns correct neighbors", "[Arena]") {
    Arena arena;
    Boid boid1({0.0f, 0.0f, 0.0f});
    Boid boid2({1.0f, 1.0f, 1.0f});
    Boid boid3({5.0f, 5.0f, 5.0f});

    arena.addBoid(&boid1);
    arena.addBoid(&boid2);
    arena.addBoid(&boid3);

    auto neighbors = arena.getNeighboringBoids(boid1, 2);
    REQUIRE(neighbors.size() == 1);
    REQUIRE(neighbors[0] == &boid2);
}

#endif