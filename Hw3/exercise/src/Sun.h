#ifndef SUN_H
#define SUN_H
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
    Color3f color;
    GLfloat orbitAngle = 0;

    void drawInternal();
    void update() override;
public:
    Sun() : Object(0.0f, 0.0f, 0.0f, false, { .red = 1.0f, .green = 0.4f, .blue = 0.0f }, TextureID::WINDOW) {}
};

#endif