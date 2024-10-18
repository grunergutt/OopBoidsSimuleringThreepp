#include "threepp/threepp.hpp"
#include "Boid.hpp"
#include <cmath>  // For std::tan and std::abs
#include <random>  // For generating random forces

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace threepp;

// Function to generate random floating-point values between a range
float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

int main() {

    Canvas canvas("Boid Simulation", {{"aa", 4}});
    GLRenderer renderer(canvas.size());
    renderer.autoClear = false;  // hud

    auto camera = PerspectiveCamera::create(50, canvas.aspect());
    camera->position.z = 5;

    OrbitControls controls{*camera, canvas};

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    // Initialize a boid at position (0, 0, 0)
    Boid myBoid(threepp::Vector3(0, 0, 0));

    // Create a mesh to represent the Boid (we'll use a Cone as a simple representation)
    auto boidGeometry = ConeGeometry::create(0.1f, 0.3f, 8);  // Radius, height, segments
    auto boidMaterial = MeshBasicMaterial::create();
    boidMaterial->color = Color::green;  // Set the color of the boid
    auto boidMesh = Mesh::create(boidGeometry, boidMaterial);

    // Add the Boid mesh to the scene
    scene->add(boidMesh);

    // Set up the HUD (optional)
    auto hud = std::make_unique<HUD>(canvas.size());
    FontLoader fontLoader;
    const auto font = fontLoader.defaultFont();
    TextGeometry::Options opts(font, 20, 5);
    const auto hudText = Text2D::create(opts, "Boid Simulation");
    hudText->setColor(Color::gray);
    hud->add(hudText, HUD::Options()
                         .setNormalizedPosition({1, 1})
                         .setHorizontalAlignment(HUD::HorizontalAlignment::RIGHT)
                         .setVerticalAlignment(HUD::VerticalAlignment::TOP));

    // Handle window resize
    canvas.onWindowResize([&](const WindowSize &size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);

        hud->setSize(size);
    });

    Clock clock;
    canvas.animate([&] {
        const auto dt = clock.getDelta();

        // Apply a smaller random force to the boid to simulate random movement
        threepp::Vector3 randomForce(
            getRandomFloat(-0.002f, 0.002f),  // Smaller x-direction force
            getRandomFloat(-0.002f, 0.002f),  // Smaller y-direction force
            0.0f);  // No z-direction force
        myBoid.applyForce(randomForce);

        // Update the boid's position and velocity
        myBoid.update();

        // Convert the camera's FOV from degrees to radians manually
        float fovInRadians = camera->fov * (M_PI / 180.0f);  // Manual conversion to radians

        // Calculate the height and width based on the camera's position and FOV
        float halfHeight = std::tan(fovInRadians * 0.5f) * std::abs(camera->position.z);
        float halfWidth = halfHeight * camera->aspect;

        // Constrain the boid within the camera's visible area (bounce off walls)
        myBoid.constrainToBorders(-halfWidth, halfWidth, -halfHeight, halfHeight);

        // Update the boid mesh to reflect the new position
        boidMesh->position.set(myBoid.position.x, myBoid.position.y, myBoid.position.z);

        // Rotate the boid mesh to face the direction of velocity
        if (myBoid.velocity.length() > 0) {
            threepp::Vector3 direction = myBoid.velocity.clone().normalize();
            boidMesh->lookAt(
                myBoid.position.x + direction.x,
                myBoid.position.y + direction.y,
                myBoid.position.z + direction.z);
        }

        // Render the scene
        renderer.clear();
        renderer.render(*scene, *camera);
        hud->apply(renderer);
    });

    return 0;
}
