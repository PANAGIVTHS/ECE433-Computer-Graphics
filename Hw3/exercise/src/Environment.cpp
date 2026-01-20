#include "Environment.h"
#include "Object.h"
#include "Celestial.h"

Environment::Environment(Color3f skyColor) {
    this->skyColor = skyColor;
}

void Environment::init() {
    glClearColor(this->skyColor.red, this->skyColor.green, this->skyColor.blue, 0);
}

void Environment::spawn() {
    new Sun();
    new Moon();
    //TODO add to settings menu
    (new Cuboid(0, -0.05, 0, 100, 0.1, 100, false, {.red = 1.0f, .green = 1.0f, .blue = 1.0f}, TextureID::MYCELIUM, MaterialID::MATTE, TextureConfig(TextureMode::REPEAT_FIT, 100, 100), 300))->optimize();

}

void Environment::updateSky(float heightFactor) {
    Color3f night   = {0.02f, 0.02f, 0.1f};
    Color3f sunset  = {0.8f, 0.4f, 0.2f};
    Color3f day     = {0.4f, 0.6f, 0.9f};

    Color3f currentSky;
    if (heightFactor > 0.0f) {
        float t = heightFactor;
        currentSky.red   = sunset.red   + (day.red   - sunset.red)   * t;
        currentSky.green = sunset.green + (day.green - sunset.green) * t;
        currentSky.blue  = sunset.blue  + (day.blue  - sunset.blue)  * t;
    } else {
        float t = (heightFactor + 0.5f) * 2.0f;
        if (t < 0.0f) t = 0.0f;
        currentSky.red   = night.red   + (sunset.red   - night.red)   * t;
        currentSky.green = night.green + (sunset.green - night.green) * t;
        currentSky.blue  = night.blue  + (sunset.blue  - night.blue)  * t;
    }

    glClearColor(currentSky.red, currentSky.green, currentSky.blue, 1.0f);
}