#include "Sun.h"
#include "GameManager.h"
#include "LightingManager.h"
#include <math.h>

LightConfig Sun::getConfig() {
    LightConfig sun;
    sun.isDirectional = true;
    sun.position = Vec3<float>(transform.position.x, transform.position.x, transform.position.z);
    sun.color = Vec3<float>(color.red, color.green, color.blue);
    return sun;
}

void Sun::drawInternal() {
    LightingManager::updateLight(lightID, getConfig());
    glDisable(GL_LIGHTING);
    glColor3f(color.red, color.green, color.blue);
    glutSolidSphere(radius, 20, 20);
    glEnable(GL_LIGHTING);
}

void Sun::update() {
    Camera* cam = GameManager::getCamera();
    if (!cam) return;

    Vec3<GLfloat> camPos = cam->getPosition();

    orbitAngle += orbitSpeed;
    GLfloat xOffset = cos(orbitAngle) * orbitRadius;
    GLfloat yOffset = sin(orbitAngle) * orbitRadius;
    
    transform.position = camPos;
    transform.position += Vec3(xOffset, yOffset, 0.0f);

    float heightFactor = yOffset / orbitRadius;
    if (heightFactor < 0.0f) heightFactor = 0.0f;

    color.red = 1.0f; 

    if (heightFactor < 0.5f) {
        float t = heightFactor * 2.0f; 
        color.green = 0.4f + (0.6f * t);
        color.blue  = 0.0f;
    } else {
        color.green = 1.0f;
        float t = (heightFactor - 0.5f) * 2.0f;
        color.blue = 0.0f + (1.0f * t);
    }
}