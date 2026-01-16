#ifndef SRC_LANTERN_H
#define SRC_LANTERN_H
#include "../Object.h"
#include "../LightingManager.h"

class Lantern : public Object {
    GLint lightID;
    LightConfig config;

    void setup();
public:
    Lantern(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { setup(); }

    void drawInternal() override;
};

#endif //SRC_LANTERN_H