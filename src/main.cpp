#include "threepp/threepp.hpp"
#include "arena.cpp"
#include "boid.cpp"
#include "flock.cpp"
#include <imgui.h>
#include <GLFW/glfw3.h>



int main() {
    int numberOfBoids = 20;
    Flock flock;
    for (int i = 0; i < numberOfBoids; i++) {
        Boid boidFlock1;
        flock.flockAddBoid(boidFlock1);
    }



    //{"width", arena.getArenaWidth()}, {"height", arena.getArenaHeight()}, {"depth", arena.getArenaDepth()},
    return 0;
}