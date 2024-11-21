#ifndef THREEPPFUNCTIONS_HPP
#define THREEPPFUNCTIONS_HPP

#include "threepp/threepp.hpp"
#include "flock.hpp"
#include <memory>
#include <string>

std::shared_ptr<threepp::Mesh> createLineMesh(
    const threepp::Vector3 &pos,
    const threepp::Color &color,
    float width,
    float height,
    float depth
);

// Declaration of createBoxMesh as a standalone function
std::shared_ptr<threepp::Group> createBoxMesh(
    const threepp::Vector3 &pos,
    const threepp::Color &color,
    float width = 1.0f,
    float height = 1.0f,
    float depth = 1.0f,
    bool wireframe = false
);

// Declaration of create2dHUD as a standalone function
std::unique_ptr<threepp::HUD> create2dHUD(
    threepp::WindowSize size,
    const std::string &writtenText,
    threepp::HUD::HorizontalAlignment horizontalPlacement,
    threepp::HUD::VerticalAlignment verticalPlacement
);

std::shared_ptr<threepp::Mesh> createConeMeshForBoid(const threepp::Vector3 &pos, const threepp::Color &color);

std::shared_ptr<threepp::Group> createAnimationGroup(
    Flock& flock,
    const threepp::Color& color,
    std::vector<std::shared_ptr<threepp::Mesh>>& boidCones
);

void rotateConeTowardsVelocity(std::shared_ptr<threepp::Mesh> boidCone, const threepp::Vector3& velocity);



#endif // THREEPPFUNCTIONS_HPP
