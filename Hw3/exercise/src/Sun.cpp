#include "Sun.h"
#include "GameManager.h"
#include <math.h>

void Sun::drawInternal() {
    glColor3f(color.red, color.green, color.blue);
    glutSolidSphere(radius, 20, 20);
}

void Sun::update() {
    Camera* cam = GameManager::getCamera();
    if (!cam) return;

    Vec3<GLfloat> camPos = cam->getPosition();

    orbitAngle += orbitSpeed;
    GLfloat xOffset = cos(orbitAngle) * orbitRadius;
    GLfloat yOffset = sin(orbitAngle) * orbitRadius;
    
    position = camPos;
    position += Vec3(xOffset, yOffset, 0.0f);
}