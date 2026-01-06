#include "LightingManager.h"

void LightingManager::init() {
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void LightingManager::startLightSource(int id, const Vec3<float>& position, const Vec3<float>& color) {
    GLenum lightEnum = GL_LIGHT0 + id; 

    glEnable(lightEnum);
    glColor3f(color.x, color.y, color.z);

    GLfloat lightPos[] = { position.x, position.y, position.z, 1.0f }; 
    glLightfv(lightEnum, GL_POSITION, lightPos);

    GLfloat ambient[]  = { color.x * 0.1f, color.y * 0.1f, color.z * 0.1f, 1.0f };
    GLfloat diffuse[]  = { color.x, color.y, color.z * 0.9f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(lightEnum, GL_AMBIENT,  ambient);
    glLightfv(lightEnum, GL_DIFFUSE,  diffuse);
    glLightfv(lightEnum, GL_SPECULAR, specular);

    glDisable(GL_LIGHTING);
}

void LightingManager::endLightSource() {
    glEnable(GL_LIGHTING);
}