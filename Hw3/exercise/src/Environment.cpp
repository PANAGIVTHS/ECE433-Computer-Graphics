#include "Environment.h"

Environment::Environment(Color3f skyColor) {
    this->skyColor = skyColor;
}

void Environment::apply() {
    glClearColor(this->skyColor.red, this->skyColor.green, this->skyColor.blue, 0);
}