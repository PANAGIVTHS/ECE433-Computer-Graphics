#include "Environment.h"
#include "Object.h"
#include "Sun.h"

Environment::Environment(Color3f skyColor) {
    this->skyColor = skyColor;
}

void Environment::init() {
    glClearColor(this->skyColor.red, this->skyColor.green, this->skyColor.blue, 0);
}

void Environment::spawn() {
    new Sun();

    float step = 1.0f;
    for (float x = -50; x <= 50; x += step) {
        for (float z = -50; z <= 50; z += step) {
            new Cuboid(x, -0.05, z, step, 0.1, step, false, {.red = 1.0f, .green = 1.0f, .blue = 1.0f}, TextureID::MYCELIUM, MaterialID::MATTE, TextureConfig(TextureMode::REPEAT_FIT));
        }
    }
}