#ifndef SRC_HOUSE_H
#define SRC_HOUSE_H
#include "../Object.h"
#include "Garage.h"

class House : public Object {
    void setup();
public:
    House(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { setup(); }
};

#endif //SRC_HOUSE_H