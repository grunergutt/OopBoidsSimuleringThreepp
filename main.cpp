#include "arena.hpp"           // Your custom class
#include "boid.hpp"            // Your custom class
#include "flock.hpp"           // Your custom class
#include "pack.hpp"            // Your custom class
#include "predator.hpp"        // Your custom class
#include "threepp/threepp.hpp" // Threepp core
#include "threeppfunctions.hpp"// Your custom functions
#include <GLFW/glfw3.h>        // GLFW library
#include <imgui.h>             // ImGui core
#include <imgui_impl_glfw.h>   // ImGui GLFW backend
#include <imgui_impl_opengl3.h>// ImGui OpenGL3 backend
#include <iostream>
#include <utilityfunctions.hpp>


int main() {

    int numberOfBoidsFlock1 = 120;
    Flock flock1;
    for (int i = 0; i < numberOfBoidsFlock1; i++) {
        flock1.flockAddBoid(std::make_unique<Boid>(i));
    }

    int numberOfBoidsFlock2 = 120;
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

    int numberOfPredatorsPack1 = 3;
    for (int i = 0; i < numberOfPredatorsPack1; i++) {
        pack1.packAddPredator(std::make_unique<Predator>(i));
    }

    threepp::Canvas canvas("threepp Boid And Predator Animation", {{"aa", 4}});
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
    std::vector<std::shared_ptr<threepp::Mesh>> predatorCones;

    auto flock1Group = createAnimationGroupForFlock(flock1, threepp::Color::yellow, boidCones1, 1);
    auto flock2Group = createAnimationGroupForFlock(flock2, threepp::Color::cyan, boidCones2, 1);
    auto flock3Group = createAnimationGroupForFlock(flock3, threepp::Color::darkblue, boidCones3, 1);
    auto predatorsGroup = createAnimationGroupForPack(pack1, threepp::Color::red, predatorCones, 4);

    scene->add(flock1Group);
    scene->add(flock2Group);
    scene->add(flock3Group);
    scene->add(predatorsGroup);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)canvas.windowPtr(), true); // Correctly retrieve window handle
    ImGui_ImplOpenGL3_Init("#version 150");
    controls.enabled = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

    // Slider for user input

    float setSeparationStrength = 10, previousSeparationStrength = setSeparationStrength;
    float setAlignmentStrength = 0.15f, previousAlignmentStrength = setAlignmentStrength;
    float setCohesionStrength = 0.1f, previousCohesionStrength = setCohesionStrength;
    int setSeparationRadius = 10, previousSeparationRadius = setSeparationRadius;
    int setAlignmentRadius = 15, previousAlignmentRadius = setAlignmentRadius;
    int setCohesionRadius = 15, previousCohesionRadius = setCohesionRadius;

    float setBoidSpeed = 20, previousBoidSpeed = setBoidSpeed;
    float setBoidForce = 4, previousBoidForce = setBoidForce;
    float setBoidRandomForce = 0.4, previousBoidRandomForce = setBoidRandomForce;

    bool simulatePredators = true;
    std::vector<threepp::Vector3> lastPredatorPositions(pack1.packGetNumPredators(), threepp::Vector3());
    for (int i = 0; i < pack1.packGetNumPredators(); i++) {
        Predator& predator = pack1.packGetPredatorByIndex(i);
        lastPredatorPositions[i] = predator.predatorGetPosition(); // Initialize with current positions
    }

    bool simulationSwitch = true;

    threepp::Clock clock;
    canvas.animate([&] {
        controls.enabled = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Flock Control Panel");
        ImGui::SliderFloat("separationStrength", &setSeparationStrength, 5, 15);
        ImGui::SliderFloat("alignmentStrength", &setAlignmentStrength, 0.05, 0.25);
        ImGui::SliderFloat("cohesionStrength", &setCohesionStrength, 0.05, 0.2);
        ImGui::SliderInt("separationRadius", &setSeparationRadius, 5, 25);
        ImGui::SliderInt("alignmentRadius", &setAlignmentRadius, 10, 30);
        ImGui::SliderInt("cohesionRadius", &setCohesionRadius, 10, 30);

        if (setSeparationStrength != previousSeparationStrength) {
            float separationDifference = setSeparationStrength - previousSeparationStrength;
            for (auto& flock : flocks) {
                flock->setSeparationStrength(flock->getSeparationStrength() + separationDifference);
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

        ImGui::Begin("Boid Control Panel");
        ImGui::SliderFloat("Max Speed", &setBoidSpeed, 10, 30);
        ImGui::SliderFloat("Max Force", &setBoidForce, 2, 6);
        ImGui::SliderFloat("Random Movement", &setBoidRandomForce, 0.0, 0.8);

        if (setBoidSpeed != previousBoidSpeed) {
            for (auto& flock : flocks) {
                for (const auto& boid : flock->getBoids()) {
                    boid->boidSetMaxSpeed(static_cast<float>(setBoidSpeed));
                    }
                }
            previousBoidSpeed = setBoidSpeed;  // Update previous value
        }

        if (setBoidForce != previousBoidForce) {
            for (auto& flock : flocks) {
                for (const auto& boid : flock->getBoids()) {
                    boid->boidSetMaxForce(static_cast<float>(setBoidForce));
                    }
                }
            previousBoidForce = setBoidForce;  // Update previous value
        }

        if (setBoidRandomForce != previousBoidRandomForce) {
            for (auto& flock : flocks) {
                for (const auto& boid : flock->getBoids()) {
                    boid->boidSetRandomForce(static_cast<float>(setBoidRandomForce));
                    }
                }
            previousBoidRandomForce = setBoidRandomForce;  // Update previous value
        }

        ImGui::End();

        ImGui::Begin("Predator Control Panel");
        ImGui::Checkbox("Simulate Predators", &simulatePredators);

        ImGui::End();


        flock1.flockUpdateFlock();
        flock2.flockUpdateFlock();
        flock3.flockUpdateFlock();
        if (simulatePredators) {
            pack1.packUpdatePack(flocks);
        }

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

        if (simulatePredators && simulationSwitch) {
            for (int i = 0; i < pack1.packGetNumPredators(); i++) {
                Predator& predator = pack1.packGetPredatorByIndex(i);

        // Randomize position within arena boundaries
                threepp::Vector3 randomPosition{
                    getRandomFloat(-arena.getArenaHeight()/2, arena.getArenaHeight()/2),
                    getRandomFloat(-arena.getArenaWidth()/2, arena.getArenaWidth()/2),
                    getRandomFloat(-arena.getArenaDepth()/2, arena.getArenaDepth()/2)
                };

                predator.predatorSetPosition(randomPosition);
                predatorCones[i]->position.copy(randomPosition); // Update visuals
         }
            simulationSwitch = false; // Prevent further reinitialization
        }

        if (simulatePredators && !simulationSwitch) {
            for (int i = 0; i < pack1.packGetNumPredators(); i++) {
                const Predator& predator = pack1.packGetPredatorByIndex(i);
                threepp::Vector3 predatorPosition = predator.predatorGetPosition();
                threepp::Vector3 predatorVelocity = predator.predatorGetVelocity();

                predatorCones[i]->position.copy(predatorPosition);
                rotateConeTowardsVelocity(predatorCones[i], predatorVelocity);
            }
        }

        if (!simulatePredators) {
            for (int i = 0; i < pack1.packGetNumPredators(); i++) {
            Predator& predator = pack1.packGetPredatorByIndex(i);

            // Save the current position
            lastPredatorPositions[i] = predator.predatorGetPosition();

            // Move predator off-screen
            threepp::Vector3 offScreen(1000, 1000, 1000);
            predator.predatorSetPosition(offScreen);
            predatorCones[i]->position.copy(offScreen);
                simulationSwitch = true;
            }
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