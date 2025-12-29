#include "Environment.h"
#include "Object.h"

Environment::Environment(Color3f skyColor) {
    this->skyColor = skyColor;
}

void Environment::spawn() {
    glClearColor(this->skyColor.red, this->skyColor.green, this->skyColor.blue, 0);
    new Terrain(0, 0, 0);
}