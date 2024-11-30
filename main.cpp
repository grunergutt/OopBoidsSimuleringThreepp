#include "threepp/threepp.hpp"      // Threepp core
#include "arena.hpp"                // Your custom class
#include "boid.hpp"                 // Your custom class
#include "flock.hpp"                // Your custom class
#include "threeppfunctions.hpp"     // Your custom functions
#include "predator.hpp"             // Your custom class
#include "pack.hpp"                 // Your custom class
#include <imgui.h>                  // ImGui core
#include <imgui_impl_glfw.h>        // ImGui GLFW backend
#include <imgui_impl_opengl3.h>     // ImGui OpenGL3 backend
#include <iostream>
#include <GLFW/glfw3.h>             // GLFW library


int main() {

    int numberOfBoidsFlock1 = 1;
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


    std::vector<Flock*> flocks = {&flock1, &flock2, &flock3};

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
    auto flock3Group = createAnimationGroupForFlock(flock3, threepp::Color::darkblue, boidCones3, 1);
    auto predatorsGroup = createAnimationGroupForPack(pack1, threepp::Color::red, predators, 4);

    scene->add(flock1Group);
    scene->add(flock2Group);
    scene->add(flock3Group);
    scene->add(predatorsGroup);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)canvas.windowPtr(), true); // Correctly retrieve window handle
    ImGui_ImplOpenGL3_Init("#version 150");

    // Slider for user input

    float setSeparationStrength = 10, previousSeparationStrength = setSeparationStrength;
    float setAlignmentStrength = 0.15f, previousAlignmentStrength = setAlignmentStrength;
    float setCohesionStrength = 0.1f, previousCohesionStrength = setCohesionStrength;
    int setSeparationRadius = 10, previousSeparationRadius = setSeparationRadius;
    int setAlignmentRadius = 15, previousAlignmentRadius = setAlignmentRadius;
    int setCohesionRadius = 15, previousCohesionRadius = setCohesionRadius;

    threepp::Clock clock;
    canvas.animate([&] {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Add ImGui slider
        ImGui::Begin("Flock Control Panel");
        ImGui::SliderFloat("separationStrength", &setSeparationStrength, 5, 15);
        ImGui::SliderFloat("alignmentStrength", &setAlignmentStrength, 0.05, 0.25);
        ImGui::SliderFloat("cohesionStrength", &setCohesionStrength, 0.05, 0.2);
        ImGui::SliderInt("separationRadius", &setSeparationRadius, 5, 25);
        ImGui::SliderInt("alignmentRadius", &setAlignmentRadius, 10, 30);
        ImGui::SliderInt("cohesionRadius", &setCohesionRadius, 10, 30);

        if (setSeparationStrength != previousSeparationStrength) {
        int separationDifference = setSeparationStrength - previousSeparationStrength;
        for (auto& flock : flocks) {
            flock->setSeparationRadius(flock->getSeparationRadius() + separationDifference);
        }
        previousSeparationStrength = setSeparationStrength; // Update previous value
    }

    if (setAlignmentStrength != previousAlignmentStrength) {
        float alignmentDifference = setAlignmentStrength - previousAlignmentStrength;
        for (auto& flock : flocks) {
            flock->setAlignmentStrength(flock->getAlignmentStrength() + alignmentDifference);
        }
        previousAlignmentStrength = setAlignmentStrength; // Update previous value
    }

    if (setCohesionStrength != previousCohesionStrength) {
        float cohesionDifference = setCohesionStrength - previousCohesionStrength;
        for (auto& flock : flocks) {
            flock->setCohesionStrength(flock->getCohesionStrength() + cohesionDifference);
        }
        previousCohesionStrength = setCohesionStrength; // Update previous value
    }

    if (setSeparationRadius != previousSeparationRadius) {
        int separationRadiusDifference = setSeparationRadius - previousSeparationRadius;
        for (auto& flock : flocks) {
            flock->setSeparationRadius(flock->getSeparationRadius() + separationRadiusDifference);
        }
        previousSeparationRadius = setSeparationRadius; // Update previous value
    }

    if (setAlignmentRadius != previousAlignmentRadius) {
        int alignmentRadiusDifference = setAlignmentRadius - previousAlignmentRadius;
        for (auto& flock : flocks) {
            flock->setAlignmentRadius(flock->getAlignmentRadius() + alignmentRadiusDifference);
        }
        previousAlignmentRadius = setAlignmentRadius; // Update previous value
    }

    if (setCohesionRadius != previousCohesionRadius) {
        int cohesionRadiusDifference = setCohesionRadius - previousCohesionRadius;
        for (auto& flock : flocks) {
            flock->setCohesionRadius(flock->getCohesionRadius() + cohesionRadiusDifference);
        }
        previousCohesionRadius = setCohesionRadius; // Update previous value
    }
        ImGui::End();

        flock1.flockUpdateFlock();
        flock2.flockUpdateFlock();
        flock3.flockUpdateFlock();
        pack1.packUpdatePack(flocks);

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


        for (int i = 0; i < pack1.packGetNumPredators(); i++) {
            const Predator& predator = pack1.packGetPredatorByIndex(i);
            threepp::Vector3 predatorPosition = predator.predatorGetPosition();
            threepp::Vector3 predatorVelocity = predator.predatorGetVelocity();

            predators[i]->position.copy(predatorPosition);
            rotateConeTowardsVelocity(predators[i], predatorVelocity);

        }

        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    });

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}