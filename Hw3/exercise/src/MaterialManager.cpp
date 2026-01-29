#include "MaterialManager.h"

void MaterialManager::init() {
    //TODO add Matte material to list in this idk the values
    //TODO important implement NONE material as default ambient diffuse...
    materials[MaterialID::NONE] = {
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.8f, 0.8f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        0.0f
    };
    materials[MaterialID::MATTE] = {
        {0.2, 0.2, 0.2, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {0.0, 0.0, 0.0, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        0.0f
    };
    materials[MaterialID::SHINY] = {
        {0.25f, 0.25f, 0.25f, 1.0f},
        {0.4f, 0.4f, 0.4f, 1.0f},
        {0.2f, 0.2f, 0.2f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        0.0f
    };
    materials[MaterialID::COLD_LIGHT] = {
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        100.0f
    };
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