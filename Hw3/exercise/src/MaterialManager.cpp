#include "MaterialManager.h"

void MaterialManager::init() {
    //TODO add Matte material to list in this idk the values
    //TODO important implement NONE material as default ambient diffuse...
    // materials[MaterialID::NONE] = {
    //     {0.2f, 0.2f, 0.2f, 1.0f}, // Ambient
    //     {0.8f, 0.8f, 0.8f, 1.0f}, // Diffuse
    //     {0.0f, 0.0f, 0.0f, 1.0f}, // Specular (No shine)
    //     {0.0f, 0.0f, 0.0f, 1.0f}, // Emission (No glow)
    //     0.0f                      // Shininess (Flat)
    // };

    // materials[MaterialID::NONE] = {
    //     {0.24725f, 0.1995f, 0.0745f, 1.0f},    // Ambient
    //     {0.75164f, 0.60648f, 0.22648f, 1.0f},  // Diffuse
    //     {0.62828f, 0.5558f, 0.366065f, 1.0f},  // Specular
    //     {0.0f, 0.0f, 0.0f, 1.0f},              // Emission
    //     51.2f                                  // Shininess (ranges 0.0 to 128.0)
    // };
}

void MaterialManager::bind(MaterialID id) {
    if (materials.find(id) == materials.end()) return;

    const Material& mat = materials[id];

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.emission);
    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
}