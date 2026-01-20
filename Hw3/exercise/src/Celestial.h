#ifndef SUN_H
#define SUN_H
#include "LightingManager.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Object.h"
#include <cmath>

class Celestial : public Object {
protected:
    static constexpr GLfloat orbitSpeed = 0.001f;
    static constexpr GLfloat orbitRadius = 100.0f;
    static constexpr GLfloat radius = 13.0f;
    GLint size;
    GLint lightID;
    GLfloat orbitAngle = 0;

    virtual LightConfig getConfig();
    void drawInternal() override;
    void update() override;
public:
    Celestial(Color3f color, GLint size) : Object(0.0f, 0.0f, 0.0f, false, color) {
        this->size = size;
        lightID = LightingManager::createLight(getConfig());
    }
};

class Sun : public Celestial {
public:
    Sun() : Celestial({ .red = 1.0f, .green = 0.4f, .blue = 0.0f }, 15) { }
};

class Moon : public Celestial {
    LightConfig getConfig() override;
    void update() override;
public:
    Moon() : Celestial({ .red = 1.0f, .green = 1.0f, .blue = 1.0f }, 15) { orbitAngle = M_PI; }
};

#endif