#include "threepp/threepp.hpp"
#include "arena.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include <imgui.h>
#include <GLFW/glfw3.h>



int main() {

    int numberOfBoids = 20;
    Flock flock;
    for (int i = 0; i < numberOfBoids; i++) {
        Boid boid;
        flock.flockAddBoid(boid);
    }

    threepp::Canvas canvas("Boids Simulation", { {"aa", 4}});
    threepp::GLRenderer renderer(canvas.size());
    renderer.autoClear(false);

    auto scene = std::make_shared<threepp::Scene>();
    auto camera = std::make_shared<threepp::PerspectiveCamera>(75, canvas.size().aspect(), 0.1f, 500.0f);

    const auto hud = threepp::createHUD(canvas.size());

    camera->position.set(0, 0, arena.getArenaDepth() * 1.5);
    camera->lookAt(threepp::Vector3(0, 0, 0));
    scene->add(camera);

    threepp::Clock clock;
    canvas.animate([&] {
        const auto dt = clock.getDelta();

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });




    //{"width", arena.getArenaWidth()}, {"height", arena.getArenaHeight()}, {"depth", arena.getArenaDepth()},
    return 0;
}