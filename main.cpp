#include "threepp/threepp.hpp"
#include "arena.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include "threeppfunctions.hpp"
#include "pack.hpp"
#include <imgui.h>
#include <iostream>
#include <GLFW/glfw3.h>


int main() {

    int numberOfBoidsFlock1 = 20;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        flock1.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfBoidsFlock2 = 0;
    Flock flock2;
    for (int i = 0; i < numberOfBoidsFlock2; i++) {
        flock2.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfBoidsFlock3 = 0;
    Flock flock3;
    for (int i = 0; i < numberOfBoidsFlock3; i++) {
        flock3.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfBoidsFlock4 = 0;
    Flock flock4;
    for (int i = 0; i < numberOfBoidsFlock4; i++) {
        flock4.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfPredatorsPack1 = 0;
    for (int i = 0; i < numberOfPredatorsPack1; i++) {
        pack1.packAddPredator(std::make_unique<Predator>(i));
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
    groupWallBorders->add(createBoxMesh(yWallPosition, threepp::Color::purple, AVBHW, 0, AVBHW, true));
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
    std::vector<std::shared_ptr<threepp::Mesh>> boidCones3;
    std::vector<std::shared_ptr<threepp::Mesh>> boidCones4;
    std::vector<std::shared_ptr<threepp::Mesh>> predators;

    auto flock1Group = createAnimationGroupForFlock(flock1, threepp::Color::yellow, boidCones1, 1);
    auto flock2Group = createAnimationGroupForFlock(flock2, threepp::Color::cyan, boidCones2, 1);
    auto flock3Group = createAnimationGroupForFlock(flock3, threepp::Color::purple, boidCones3, 1);
    auto flock4Group = createAnimationGroupForFlock(flock4, threepp::Color::orange, boidCones4, 1);
    auto predatorsGroup = createAnimationGroupForPack(pack1, threepp::Color::red, predators, 4);

    scene->add(flock1Group);
    scene->add(flock2Group);
    scene->add(flock3Group);
    scene->add(flock4Group);
    scene->add(predatorsGroup);

    threepp::Clock clock;
    canvas.animate([&] {
        flock1.flockUpdateFlock();
        flock2.flockUpdateFlock();
        flock3.flockUpdateFlock();
        flock4.flockUpdateFlock();

        for (int i = 0; i < flock1.flockGetNumBoids(); i++) {
            const Boid& boid = flock1.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();
            threepp::Vector3 boidVelocity = boid.boidGetVelocity();

            boidCones1[i]->position.copy(boidPosition);
            rotateConeTowardsVelocity(boidCones1[i], boidVelocity);
        }

        for (int i = 0; i < flock2.flockGetNumBoids(); i++) {
            const Boid& boid = flock2.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();
            threepp::Vector3 boidVelocity = boid.boidGetVelocity();

            boidCones2[i]->position.copy(boidPosition);
            rotateConeTowardsVelocity(boidCones2[i], boidVelocity);
        }

        for (int i = 0; i < flock3.flockGetNumBoids(); i++) {
            const Boid& boid = flock3.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();
            threepp::Vector3 boidVelocity = boid.boidGetVelocity();

            boidCones3[i]->position.copy(boidPosition);
            rotateConeTowardsVelocity(boidCones3[i], boidVelocity);
        }

        for (int i = 0; i < flock4.flockGetNumBoids(); i++) {
            const Boid& boid = flock4.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();
            threepp::Vector3 boidVelocity = boid.boidGetVelocity();

            boidCones4[i]->position.copy(boidPosition);
            rotateConeTowardsVelocity(boidCones4[i], boidVelocity);
        }

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });

    return 0;
}