#include "threepp/threepp.hpp"
#include "arena.cpp"
#include "boid.cpp"
#include "flock.cpp"
#include "threeppfunctions.cpp"
#include <imgui.h>
#include <GLFW/glfw3.h>


int main() {

    int numberOfBoids = 20;
    Flock flock;
    for (int i = 0; i < numberOfBoids; i++) {
        Boid boidFlock1;
        flock.flockAddBoid(boidFlock1);
    }

    threepp::Canvas canvas("threepp demo", {{"aa", 4}});
    threepp::GLRenderer renderer(canvas.size());
    renderer.autoClear = false;

    auto camera = threepp::PerspectiveCamera::create(70, canvas.aspect());
    camera->position.z = 60;
    camera->position.x = 60;
    camera->position.y = 60;
    camera->lookAt({0, 0, 0});

    threepp::OrbitControls controls{*camera, canvas};

    auto scene = threepp::Scene::create();
    scene->background = threepp::Color::aliceblue;

    float AVBHW = arena.getArenaHeight();    //arenaVisualBorderHeightWidth

    threepp::Vector3 xWallPosition = threepp::Vector3(0, 0, -1*arena.getArenaHeight()/2);
    threepp::Vector3 yWallPosition = threepp::Vector3(0, -1*arena.getArenaHeight() / 2, 0);
    threepp::Vector3 zWallPosition = threepp::Vector3(-1*arena.getArenaHeight() / 2,0 , 0);
    threepp::Vector3 zeroWallPosition = threepp::Vector3(0,0 , 0);

    auto group = threepp::Group::create();
    group->add(createBoxMesh(zeroWallPosition, threepp::Color::red, 1, 1, 1, false));
    group->add(createBoxMesh(xWallPosition, threepp::Color::green, AVBHW, AVBHW, 0, true));
    group->add(createBoxMesh(yWallPosition, threepp::Color::green, AVBHW, 0, AVBHW, true));
    group->add(createBoxMesh(zWallPosition, threepp::Color::green, 0, AVBHW, AVBHW, true));
    scene->add(group);

    const auto hud = create2dHUD(canvas.size(), "Boid Simulation", threepp::HUD::HorizontalAlignment::LEFT, threepp::HUD::VerticalAlignment::TOP);

    canvas.onWindowResize([&](const threepp::WindowSize &size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
        hud->setSize(size);
    });


    threepp::Clock clock;
    canvas.animate([&] {

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });

    return 0;
}