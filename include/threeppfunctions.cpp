#include "threeppfunctions.hpp"

// Define createBoxMesh as a standalone function
std::shared_ptr<threepp::Mesh> createBoxMesh(
    const threepp::Vector3 &pos,
    const threepp::Color &color,
    float width,
    float height,
    float depth,
    bool grid
) {
    const auto geometry = threepp::BoxGeometry::create(width, height, depth);

    threepp::TextureLoader textureLoader;
    auto gridTexture = textureLoader.load("C:/OOPc++/OopBoidsSimuleringThreepp/images/GridTextureForWalls/GridTextureForWalls.jpg");
// lenket til bildet brukt.
// https://www.google.com/search?q=grid+texture&client=opera-gx&hs=QEL&sca_esv=afec391d6f6dcabb&udm=2&biw=1495&bih=746&sxsrf=ADLYWIIVqdW75npAn1j_fHl1m5HITkInew%3A1731516787140&ei=c9k0Z9idCMHawPAPuNzQwQE&ved=0ahUKEwiYo5TY4tmJAxVBLRAIHTguNBgQ4dUDCBA&uact=5&oq=grid+texture&gs_lp=EgNpbWciDGdyaWQgdGV4dHVyZTIEECMYJzIEECMYJzIFEAAYgAQyBRAAGIAEMgUQABiABDIFEAAYgAQyBRAAGIAEMgUQABiABDIFEAAYgAQyBRAAGIAESLAQUP8DWMAJcAF4AJABAJgBZKAB6ASqAQM3LjG4AQPIAQD4AQGYAgKgArYBmAMAiAYBkgcDMS4xoAegKA&sclient=img#vhid=ZRxAefoxT_uRrM&vssid=mosaic

    const auto material = threepp::MeshBasicMaterial::create();
    if (grid) {
        material->map = gridTexture;
    }
    material->color.copy(color);

    auto mesh = threepp::Mesh::create(geometry, material);
    mesh->position.copy(pos);

    return mesh;
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
