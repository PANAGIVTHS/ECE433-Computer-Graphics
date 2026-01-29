#include "Environment.h"

#include "AssetLoader.h"
#include "Object.h"
#include "Celestial.h"
#include "Spline.h"


Environment::Environment(Color3f skyColor) {
    this->skyColor = skyColor;
}

void Environment::init() {
    glClearColor(this->skyColor.red, this->skyColor.green, this->skyColor.blue, 0);
}

void Environment::spawn() {
    Object *portal = AssetLoader::load("../assets/nether_portal.txt");
    portal->optimize();
    LightConfig config;
    config.position = {2.0f, 1.5f, 0.5f};
    config.color = {0.765625f, 0.0703125f, 0.85546875f};
    config.constant = 1.0f;
    config.linear = 0.5f;
    config.quadratic = 0.3f;
    LightingManager::registerLight(config, portal);

    static const float rockRotations[] = {286, 115, 295, 12, 181, 67};

    auto rockFunc = [](float u, float v) -> Vec3<float> {
        float r2 = u*u + v*v;
        
        float height = exp(-r2 * 4.0f); 
        
        height += 0.10f * fabs(sin(u * 10.0f) * cos(v * 10.0f)); 
        height += 0.05f * fabs(cos(u * 20.0f + v * 20.0f)); 

        if (height < 0) height = 0;
        return Vec3<float>(u, height, v);
    };
    
    new Sun();
    new Moon();
    //TODO add to settings menu
    (new Cuboid(0, -0.05, 0, 100, 0.1, 100, false, {.red = 1.0f, .green = 1.0f, .blue = 1.0f}, TextureID::GRASS, MaterialID::MATTE, TextureConfig(TextureMode::REPEAT_FIT, 100, 100), 300))->optimize();

    //! Generate the Ring of mountains
    int numSegments = 6;
    float radius = 200.0f;

    for(int i = 0; i < numSegments; i++) {
        float angle = (float)i / (float)numSegments * 2.0f * 3.14159f;
        
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        float wave = sin(angle * 5.0f); 
        float baseScale = 150.0f;
        float variation = 80.0f * wave;
        
        float finalScaleY = baseScale + variation + (rand() % 40);
        float finalScaleXZ = 180.0f;

        NurbsSurface* rock = new NurbsSurface(
            rockFunc,
            -1.0f, 1.0f, 6,
            -1.0f, 1.0f, 6,
            Vec3<float>(x, -30, z),
            4,
            {0.4f, 0.35f, 0.3f},
            TextureID::STONE, 
            MaterialID::MATTE
        );

        rock->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT, 100.0f, 100.0f));
        rock->setScale(finalScaleXZ, finalScaleY, finalScaleXZ);
        
        //! Random Rotation to break repetition
        rock->setRotation(rockRotations[i], Vec3<float>(0, 1, 0));

        rock->optimize();
    }

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