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
    static constexpr GLfloat orbitSpeed = 0.0005f;
    static constexpr GLfloat orbitRadius = 100.0f;
    static constexpr GLfloat radius = 13.0f;
    static constexpr Color3f color = {.red = 1.0f, .green = 1.0f, .blue = 0.0f};
    GLfloat orbitAngle = 0;

    void drawInternal();
    void update() override;
public:
    Sun() : Object(0.0f, 0.0f, 0.0f) {}
};

#endif