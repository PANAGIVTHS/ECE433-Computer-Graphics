#ifndef SRC_LANTERN_H
#define SRC_LANTERN_H
#include "../Object.h"
#include "../LightingManager.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
class Lantern : public Object {
    GLint lightID;
    LightConfig config;

    void setup();
public:
    Lantern(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { setup(); }
};

#endif //SRC_LANTERN_H