#include "threeppfunctions.hpp"
#include "arena.hpp"
#include "flock.hpp"
#include "boid.hpp"
#include "predator.hpp"
#include "pack.hpp"
#include <iostream>

std::shared_ptr<threepp::Mesh> createLineMesh(                                          // creates a mesh for a box, intended to
    const threepp::Vector3 &pos,                                                        // create a thin line for background walls in
    float width, float height, float depth) {                                           // animation

    auto geometry = threepp::BoxGeometry::create(
        width > 0 ? width : 0.1f,                                                       // Use a small non-zero size if the dimension is intended to be zero
        height > 0 ? height : 0.1f,                                                     // Ensures the mesh has visibility in that dimension
        depth > 0 ? depth : 0.1f
    );

    auto material = threepp::MeshBasicMaterial::create();
    material->color.copy(threepp::Color(0,0,0));

    auto lineMesh = threepp::Mesh::create(geometry, material);

    lineMesh->position.copy(pos);

    return lineMesh;
}

std::shared_ptr<threepp::Group> createBoxMesh(                          // this function stands for the creation of the arena walls
    const threepp::Vector3 &pos,                                        // it is set with these paramaters
    const threepp::Color &color,
    float width,
    float height,
    float depth,
    bool grid                                                           // if grid is true, createlinemesh is used to create a grid
                                                                        // pattern on the wall based on arenacellsize
) {

    auto group = std::make_shared<threepp::Group>();                    // creates a group so both the wall and lines are in the same one

    auto geometry = threepp::BoxGeometry::create(  // creates wall
        width, height, depth);
    auto material =
        threepp::MeshBasicMaterial::create();
    material->color.copy(color);

    auto boxMesh = threepp::Mesh::create(
        geometry, material);
    boxMesh->position.copy(pos);
    group->add(boxMesh);                                               // Add the "box" to the group

    if (grid) {                                                        // this part creates vertical and horizontal lines
        float cellSize = arena.getCellSize();                          // on the wall so the animiation has betetr viusal scale

                                                                       // since the walls are x*x*0 pixels wide, it checks which
                                                                       // coordinates are bigger than 0 units, and ignores those
        if (height > 0) {
            for (float x = -width / 2; x <= width / 2; x += cellSize) {         // since the middle of the arena is allways 0, 0, 0
                for (float z = -depth / 2; z <= depth / 2; z += cellSize) {     // the functions defines where the lines start and end by
                    threepp::Vector3 start(pos.x + x, pos.y, pos.z + z);        // using -width/2 and width/2, and the same for depth
                    auto lineMesh = createLineMesh(start,        // for each interval the distance goes up by cellsize so
                        0.1f, height, 0.1f);                         // the lines in each direction form a grid-pattern with
                    group->add(lineMesh);                                      // the apropriate size
                }
            }
        }

        if (depth > 0) {
            for (float x = -width / 2; x <= width / 2; x += cellSize) {
                for (float y = -height / 2; y <= height / 2; y += cellSize) {
                    threepp::Vector3 start(pos.x + x, pos.y + y, pos.z);
                    auto lineMesh = createLineMesh(start, 0.1f, 0.1f, depth);
                    group->add(lineMesh);
                }
            }
        }

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

    return group;                                                       // Return the group containing both the box and lines
}

std::unique_ptr<threepp::HUD> create2dHUD(                              // creates a simple text hud based on size and position
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

std::shared_ptr<threepp::Mesh> createConeMeshForObject(                     //this creates a simple cone mesh that spesifies size
    const threepp::Vector3 &pos, const threepp::Color &color, int size) {   // color and position
    float radius = 0.5f * size;
    float height = 1.5f * size;
    int radialSegments = 16;

    auto geometry =
        threepp::ConeGeometry::create(radius, height, radialSegments);      // creates a conemesh using radius and height

    auto material =
        threepp::MeshBasicMaterial::create();
    material->color.copy(color);

    auto coneMesh =
        threepp::Mesh::create(geometry, material);                          // copies material and geometry

    coneMesh->position.copy(pos);                                           // sets the position to a specified position

    coneMesh->rotation.x = -3.14159f / 2;                                   // sets default rotation to one radian

    return coneMesh;
}

std::shared_ptr<threepp::Group> createAnimationGroupForFlock(               // creates a animationgroup for a flock instance
    Flock& flock,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& boidCones,                 // uses a preexisting defined vector of meshes
    int size)
{

    auto group = threepp::Group::create();                   // defines group

    for (int i = 0; i < flock.flockGetNumBoids(); i++) {
        const Boid& boid = flock.getBoidByIndex(i);
        threepp::Vector3 boidPosition = boid.boidGetPosition();             //defines where mesh should be at

        auto boidCone =
            createConeMeshForObject(boidPosition, color, size);             // adds a boid to the group by using createconemesh

        group->add(boidCone);                                               // adds the #boidcone" to group
        boidCones.push_back(boidCone);                                      // adds the boidcone to vector of meshes
    }
    return group;
}

std::shared_ptr<threepp::Group> createAnimationGroupForPack(                // exacpt same as above, but for predator and pack
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


//chatgpt
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
