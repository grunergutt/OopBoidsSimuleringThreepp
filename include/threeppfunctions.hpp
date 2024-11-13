#ifndef THREEPPFUNCTIONS_HPP
#define THREEPPFUNCTIONS_HPP

#include "threepp/threepp.hpp"
#include <memory>
#include <string>

// Declaration of createBoxMesh as a standalone function
std::shared_ptr<threepp::Mesh> createBoxMesh(
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

#endif // THREEPPFUNCTIONS_HPP
