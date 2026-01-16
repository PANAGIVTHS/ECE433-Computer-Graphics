#include "Sun.h"
#include "GameManager.h"
#include "LightingManager.h"
#include <math.h>

LightConfig Sun::getConfig() {
    LightConfig sun;
    sun.isDirectional = true;
    sun.position = Vec3<float>(transform.position.x, transform.position.y, transform.position.z);
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
    float colorHeight = heightFactor;
    if (colorHeight < 0.0f) colorHeight = 0.0f;

    float r = 1.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (colorHeight < 0.5f) {
        // Sunset Gradient: Orange (0.4) -> Yellow (1.0)
        float t = colorHeight * 2.0f;
        g = 0.4f + (0.6f * t);
        b = 0.0f;
    } else {
        // Day Gradient: Yellow -> White
        g = 1.0f;
        float t = (colorHeight - 0.5f) * 2.0f;
        b = 0.0f + (1.0f * t);
    }

    // 4. Determine Intensity (Brightness)
    float intensity = 1.0f;
    float twilightLimit = -0.25f; // How far down until completely black

    if (heightFactor < 0.0f) {
        if (heightFactor < twilightLimit) {
            // Sun is deep underground -> Light is OFF
            intensity = 0.0f;
        } else {
            // Sun is in "Twilight" zone -> Fade from 1.0 to 0.0
            // Formula maps [0 to -0.25] to [1.0 to 0.0]
            intensity = 1.0f - (heightFactor / twilightLimit);
        }
    }

    // 5. Apply Intensity to Colors
    color.red   = r * intensity;
    color.green = g * intensity;
    color.blue  = b * intensity;
}