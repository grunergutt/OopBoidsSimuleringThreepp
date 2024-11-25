#include "threeppfunctions.hpp"
#include "arena.hpp"
#include "flock.hpp"
#include "boid.hpp"
#include "predator.hpp"
#include "pack.hpp"
#include <iostream>

std::shared_ptr<threepp::Mesh> createLineMesh(const threepp::Vector3 &pos, float width, float height, float depth) {

    auto geometry = threepp::BoxGeometry::create(
        width > 0 ? width : 0.1f,                                                                                       // Use a small non-zero size if the dimension is intended to be zero
        height > 0 ? height : 0.1f,                                                                                     // Ensures the mesh has visibility in that dimension
        depth > 0 ? depth : 0.1f
    );

    auto material = threepp::MeshBasicMaterial::create();
    material->color.copy(threepp::Color(0,0,0));

    auto lineMesh = threepp::Mesh::create(geometry, material);

    lineMesh->position.copy(pos);

    return lineMesh;
}

std::shared_ptr<threepp::Group> createBoxMesh(
    const threepp::Vector3 &pos,
    const threepp::Color &color,
    float width,
    float height,
    float depth,
    bool grid
) {

    auto group = std::make_shared<threepp::Group>();

    // Create the main box geometry and material
    auto geometry = threepp::BoxGeometry::create(width, height, depth);
    auto material = threepp::MeshBasicMaterial::create();
    material->color.copy(color);

    auto boxMesh = threepp::Mesh::create(geometry, material);
    boxMesh->position.copy(pos);
    group->add(boxMesh);  // Add the box to the group

    if (grid) {
        float cellSize = arena.getCellSize();

        // Create vertical grid lines along the XZ plane (aligned along the Y-axis)
        if (height > 0) {
            for (float x = -width / 2; x <= width / 2; x += cellSize) {
                for (float z = -depth / 2; z <= depth / 2; z += cellSize) {
                    threepp::Vector3 start(pos.x + x, pos.y, pos.z + z);
                    auto lineMesh = createLineMesh(start, 0.1f, height, 0.1f);
                    group->add(lineMesh);
                }
            }
        }

        // Create horizontal grid lines along the XY plane (aligned along the Z-axis)
        if (depth > 0) {
            for (float x = -width / 2; x <= width / 2; x += cellSize) {
                for (float y = -height / 2; y <= height / 2; y += cellSize) {
                    threepp::Vector3 start(pos.x + x, pos.y + y, pos.z);
                    auto lineMesh = createLineMesh(start, 0.1f, 0.1f, depth);
                    group->add(lineMesh);
                }
            }
        }

        // Create depth grid lines along the YZ plane (aligned along the X-axis)
        if (width > 0) {
            for (float y = -height / 2; y <= height / 2; y += cellSize) {
                for (float z = -depth / 2; z <= depth / 2; z += cellSize) {
                    threepp::Vector3 start(pos.x, pos.y + y, pos.z + z);
                    auto lineMesh = createLineMesh(start, width, 0.1f, 0.1f);
                    group->add(lineMesh);
                }
            }
        }
    }

    return group;  // Return the group containing both the box and lines
}

// Define create2dHUD as a standalone function
std::unique_ptr<threepp::HUD> create2dHUD(
    threepp::WindowSize size,
    const std::string &writtenText,
    threepp::HUD::HorizontalAlignment horizontalPlacement,
    threepp::HUD::VerticalAlignment verticalPlacement
) {
    auto hud = std::make_unique<threepp::HUD>(size);
    threepp::FontLoader fontLoader;
    const auto font = fontLoader.defaultFont();
    threepp::TextGeometry::Options opts(font, 20, 5);
    const auto hudText2 = threepp::Text2D::create(opts, writtenText);
    hudText2->setColor(threepp::Color::gray);
    hud->add(hudText2, threepp::HUD::Options()
                              .setNormalizedPosition({1, 1})
                              .setHorizontalAlignment(horizontalPlacement)
                              .setVerticalAlignment(verticalPlacement));

    return hud;
}

std::shared_ptr<threepp::Mesh> createConeMeshForObject(const threepp::Vector3 &pos, const threepp::Color &color, int size) {
    float radius = 0.5f * size;
    float height = 1.5f * size;
    int radialSegments = 16;

    auto geometry = threepp::ConeGeometry::create(radius, height, radialSegments);


    auto material = threepp::MeshBasicMaterial::create();
    material->color.copy(color);

    auto coneMesh = threepp::Mesh::create(geometry, material);

    coneMesh->position.copy(pos);

    coneMesh->rotation.x = -3.14159f / 2;

    //std::cout << "Creating cone mesh at position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;          //Debuging boid spawning

    return coneMesh;

}

std::shared_ptr<threepp::Group> createAnimationGroupForFlock(
    Flock& flock,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& boidCones,
    int size)
{

    auto group = threepp::Group::create();

    for (int i = 0; i < flock.flockGetNumBoids(); i++) {
        const Boid& boid = flock.getBoidByIndex(i);
        threepp::Vector3 boidPosition = boid.boidGetPosition();

        auto boidCone = createConeMeshForObject(boidPosition, color, size);

        group->add(boidCone);
        boidCones.push_back(boidCone);
    }

    return group;
}

std::shared_ptr<threepp::Group> createAnimationGroupForPack(
    Pack& pack,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& predatorCones,
    int size)
{

    auto group = threepp::Group::create();

    for (int i = 0; i < pack.packGetNumPredators(); i++) {
        const Predator& predator = pack.packGetPredatorByIndex(i);
        threepp::Vector3 predatorPosition = predator.predatorGetPosition();

        auto boidCone = createConeMeshForObject(predatorPosition, color, size);

        group->add(boidCone);
        predatorCones.push_back(boidCone);
    }

    return group;
}

void rotateConeTowardsVelocity(std::shared_ptr<threepp::Mesh> boidCone, const threepp::Vector3& velocity) {                     //gpt generated, didnt understand without.
    if (velocity.length() > 0.001f) { // Use a small threshold to avoid undefined behavior for near-zero velocities
        // Normalize the velocity to get the direction vector
        threepp::Vector3 direction = velocity.clone().normalize();

        // Define the cone's default "forward" direction (aligned with +Y axis in its local space)
        threepp::Vector3 defaultForward(0, 1, 0);

        // Calculate the rotation quaternion that aligns the cone's default "forward" with the velocity direction
        auto rotation = threepp::Quaternion().setFromUnitVectors(defaultForward, direction);

        // Apply the rotation to the cone
        boidCone->quaternion.copy(rotation);
    }
}
