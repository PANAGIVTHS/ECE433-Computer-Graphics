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
    (new Cuboid(0, -0.05, 0, 100, 0.1, 100, false, {.red = 1.0f, .green = 1.0f, .blue = 1.0f}, TextureID::MYCELIUM, MaterialID::MATTE, TextureConfig(TextureMode::REPEAT_FIT, 100, 100), 500))->optimize();

}