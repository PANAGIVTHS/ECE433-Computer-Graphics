#include "Celestial.h"
#include "GameManager.h"
#include "LightingManager.h"
#include <math.h>

LightConfig Celestial::getConfig() {
    LightConfig moon;
    moon.isDirectional = true;
    moon.position = Vec3<float>(transform.position.x, transform.position.y, transform.position.z);
    moon.color = Vec3<float>(color.red, color.green, color.blue);
    return moon;
}

LightConfig Moon::getConfig() {
    LightConfig config;
    config.isDirectional = true;
    config.position = transform.position;

    float dimFactor = 0.01f;

    config.color = Vec3<float>(
        color.red * dimFactor,
        color.green * dimFactor,
        color.blue * dimFactor
    );

    return config;
}

void Celestial::drawInternal() {
    LightingManager::setConfig(lightID, getConfig());
    LightingManager::updateLight(lightID);
    bool lightingOn = glIsEnabled(GL_LIGHTING);
    
    glDisable(GL_LIGHTING);
    glColor3f(color.red, color.green, color.blue);
    glutSolidSphere(radius, size, 20);
    if (lightingOn)
        glEnable(GL_LIGHTING);
}

void Celestial::update() {
    Camera* cam = GameManager::getCamera();
    if (!cam) return;

    Vec3<GLfloat> camPos = cam->getPosition();

    orbitAngle += orbitSpeed * GameManager::dt;
    GLfloat xOffset = cos(orbitAngle) * orbitRadius;
    GLfloat yOffset = sin(orbitAngle) * orbitRadius;
    
    transform.position = camPos;
    transform.position += Vec3(xOffset, yOffset, 0.0f);

    float heightFactor = yOffset / orbitRadius;
    float colorHeight = heightFactor;
    if (colorHeight < 0.0f) colorHeight = 0.0f;

    float r = 1.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (colorHeight < 0.5f) {
        float t = colorHeight * 2.0f;
        g = 0.4f + (0.6f * t);
        b = 0.0f;
    } else {
        g = 1.0f;
        float t = (colorHeight - 0.5f) * 2.0f;
        b = 0.0f + (1.0f * t);
    }

    float intensity = 1.0f;
    float twilightLimit = -0.25f;
    if (heightFactor < 0.0f) {
        if (heightFactor < twilightLimit) {
            intensity = 0.0f;
        } else {
            intensity = 1.0f - (heightFactor / twilightLimit);
        }
    }

    color.red   = r * intensity;
    color.green = g * intensity;
    color.blue  = b * intensity;

    GameManager::getEnvironment()->updateSky(heightFactor);
}

void Moon::update() {
    Camera* cam = GameManager::getCamera();
    if (!cam) return;

    Vec3<GLfloat> camPos = cam->getPosition();

    orbitAngle += orbitSpeed * GameManager::dt;
    GLfloat xOffset = cos(orbitAngle) * orbitRadius;
    GLfloat yOffset = sin(orbitAngle) * orbitRadius;

    transform.position = camPos;
    transform.position += Vec3(xOffset, yOffset, 0.0f);
}