#ifndef THREEPPFUNCTIONS_HPP
#define THREEPPFUNCTIONS_HPP

#include "threepp/threepp.hpp"


class Pack;
class Flock;

std::shared_ptr<threepp::Mesh> createLineMesh(                      // creates a mesh for a visible 1*1*x line
    const threepp::Vector3 &pos,
    const threepp::Color &color,
    float width,
    float height,
    float depth
);

std::shared_ptr<threepp::Group> createBoxMesh(                     // creates a mesh for a visible x*x*x box
    const threepp::Vector3 &pos,                                   // it is placed in a spesific position with
    const threepp::Color &color,                                   // a spesific color
    float width = 1.0f,
    float height = 1.0f,
    float depth = 1.0f,
    bool wireframe = false
);

std::unique_ptr<threepp::HUD> create2dHUD(                         // creates a 2D hud with text
    threepp::WindowSize size,
    const std::string &writtenText,
    threepp::HUD::HorizontalAlignment horizontalPlacement,
    threepp::HUD::VerticalAlignment verticalPlacement
);

std::shared_ptr<threepp::Mesh> createConeMeshForObject(            // creates a cone  mesh with a spesific
    const threepp::Vector3 &pos,                                   // color, position and size
    const threepp::Color &color, int size);

std::shared_ptr<threepp::Group> createAnimationGroupForFlock(      // creates an animationgroup for a flock
    Flock& flock,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& boidCones,
    int size
);

std::shared_ptr<threepp::Group> createAnimationGroupForPack(       // creates an animationgroup for a pack
    Pack& pack,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& predatorCones,
    int size
);

void rotateConeTowardsVelocity(                                    // rotates a conemesh towards its hosts velocity
    std::shared_ptr<threepp::Mesh>boidCone,
    const threepp::Vector3& velocity);



#endif // THREEPPFUNCTIONS_HPP
