#ifndef SUN_H
#define SUN_H
#include "LightingManager.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Object.h"

class Sun : public Object {
private:
    static constexpr GLfloat orbitSpeed = 0.001f;
    static constexpr GLfloat orbitRadius = 100.0f;
    static constexpr GLfloat radius = 13.0f;
    GLint lightID;
    Color3f color;
    GLfloat orbitAngle = 0;

    LightConfig getConfig();
    void drawInternal();
    void update() override;
public:
    Sun() : Object(0.0f, 0.0f, 0.0f, false, { .red = 1.0f, .green = 0.4f, .blue = 0.0f }) {
        lightID = LightingManager::createLight(getConfig());
    }
};

#endif