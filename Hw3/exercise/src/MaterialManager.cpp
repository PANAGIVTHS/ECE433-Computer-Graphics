#include "MaterialManager.h"

void MaterialManager::init() {
    //TODO add Matte material to list in this idk the values
    //TODO important implement NONE material as default ambient diffuse...
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