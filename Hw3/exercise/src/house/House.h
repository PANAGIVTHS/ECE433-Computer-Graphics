#ifndef SRC_HOUSE_H
#define SRC_HOUSE_H
#include "../Object.h"

class House : public Object {
    void setup();
public:
    static inline GLfloat height = 3.0f;
    static inline GLfloat interiorWallThickness = .10f;
    static inline GLfloat exteriorWallThickness = .25f;

    House(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { setup(); }
};

#endif //SRC_HOUSE_H