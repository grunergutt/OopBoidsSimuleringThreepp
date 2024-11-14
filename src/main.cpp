#include "threepp/threepp.hpp"
#include "arena.cpp"
#include "boid.cpp"
#include "flock.cpp"
#include "threeppfunctions.cpp"
#include <imgui.h>
#include <GLFW/glfw3.h>


int main() {

    int numberOfBoidsFlock1 = 1;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        Boid boidFlock1(i);
        flock1.flockAddBoid(boidFlock1);
    }

    int numberOfBoidsFlock2 = 30;
    Flock flock2;
    for (int i = 0; i < numberOfBoidsFlock2; i++) {
        Boid boidFlock2(i);
        flock2.flockAddBoid(boidFlock2);
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

    auto groupWallBorders = threepp::Group::create();
    //groupWallBorders->add(createBoxMesh(zeroWallPosition, threepp::Color::red, 1, 1, 1, false));                                      //Fjern når ikke trengt
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

    const auto flock1Group = threepp::Group::create();
    std::vector<std::shared_ptr<threepp::Mesh>> boidCones;

    for (auto i = 0; i < flock1.flockGetNumBoids(); i++) {

        const Boid& boid = flock1.getBoidByIndex(i);
        threepp::Vector3 boidPosition = boid.boidGetPosition();

        auto boidCone = createConeMeshForBoid(boidPosition, threepp::Color::yellow);

        flock1Group->add(boidCone);
        boidCones.push_back(boidCone);
    }

    scene->add(flock1Group);

    threepp::Clock clock;
    canvas.animate([&] {
        flock1.flockApplyFlockingForces();
        flock1.flockUpdateFlock();

        for (int i = 0; i < flock1.flockGetNumBoids(); i++) {
            const Boid& boid = flock1.getBoidByIndex(i);
            threepp::Vector3 boidPosition = boid.boidGetPosition();

            boidCones[i]->position.copy(boidPosition);
        }

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });

    return 0;
}