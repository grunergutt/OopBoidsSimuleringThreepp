#include "threepp/threepp.hpp"
#include "arena.cpp"
#include "boid.cpp"
#include "flock.cpp"
#include "threeppfunctions.cpp"
#include <imgui.h>
#include <iostream>
#include <GLFW/glfw3.h>


int main() {

    int numberOfBoidsFlock1 = 30;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        flock1.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfBoidsFlock2 = 0;
    Flock flock2;
    for (int i = 0; i < numberOfBoidsFlock2; i++) {
        flock2.flockAddBoid(std::make_unique<Boid>(i));
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
    scene->background = threepp::Color::orange;

    float AVBHW = arena.getArenaHeight();    //arenaVisualBorderHeightWidth

    threepp::Vector3 xWallPosition = threepp::Vector3(0, 0, -1*arena.getArenaHeight()/2);
    threepp::Vector3 yWallPosition = threepp::Vector3(0, -1*arena.getArenaHeight() / 2, 0);
    threepp::Vector3 zWallPosition = threepp::Vector3(-1*arena.getArenaHeight() / 2,0 , 0);

    auto groupWallBorders = threepp::Group::create();
    groupWallBorders->add(createBoxMesh(xWallPosition, threepp::Color::green, AVBHW, AVBHW, 0, true));
    groupWallBorders->add(createBoxMesh(yWallPosition, threepp::Color::red, AVBHW, 0, AVBHW, true));
    groupWallBorders->add(createBoxMesh(zWallPosition, threepp::Color::blue, 0, AVBHW, AVBHW, true));
    scene->add(groupWallBorders);

    const auto hud = create2dHUD(canvas.size(), "Boid Simulation", threepp::HUD::HorizontalAlignment::RIGHT, threepp::HUD::VerticalAlignment::TOP);

    canvas.onWindowResize([&](const threepp::WindowSize &size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
        hud->setSize(size);
    });

    std::vector<std::shared_ptr<threepp::Mesh>> boidCones1;
    std::vector<std::shared_ptr<threepp::Mesh>> boidCones2;

    auto flock1Group = createAnimationGroup(flock1, threepp::Color::yellow, boidCones1);
    auto flock2Group = createAnimationGroup(flock2, threepp::Color::cyan, boidCones2);

    scene->add(flock1Group);
    scene->add(flock2Group);

    threepp::Clock clock;
    canvas.animate([&] {
        flock1.flockUpdateFlock();
        flock2.flockUpdateFlock();

        for (int i = 0; i < flock1.flockGetNumBoids(); i++) {
            const Boid& boid = flock1.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();

            boidCones1[i]->position.copy(boidPosition);
        }

        for (int i = 0; i < flock2.flockGetNumBoids(); i++) {
            const Boid& boid = flock2.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();

            boidCones2[i]->position.copy(boidPosition);
        }

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });

    return 0;
}