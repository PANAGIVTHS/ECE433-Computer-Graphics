#include "Lantern.h"

void Lantern::setup() {
    float w = 0.20f;
    float h = 0.35f;
    float d = 0.20f;
    float armLength = 0.30f;

    // 1. THE ARM (Sticks out from the wall behind the lantern)
    Object* arm = new Cube(
        Vec3<float>(0.0f, h/2.0f, 0.15f),
        Vec3<float>(0.05f, 0.05f, 0.30f),
        gravity,
        {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625}
    );
    this->addChildren(arm);

    // 2. THE CAP (Roof)
    Object* cap = new Cube(
        Vec3<float>(0.0f, h/2.0f + 0.02f, 0.0f),
        Vec3<float>(w + 0.05f, 0.05f, d + 0.05f),
        gravity,
        {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625}
    );
    this->addChildren(cap);

    // 2. THE BACKPLATE
    Object* backPlate = new Cube(
        Vec3<float>(0.0f, h/2.0f, armLength),
        Vec3<float>(0.12f, 0.25f, 0.02f),
        gravity,
        {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625}
    );
    this->addChildren(backPlate);

    // 3. THE BODY (Frame)
    Object* body = new Cuboid(
        Vec3<float>(0.0f, 0.0f, 0.0f),
        Vec3<float>(w, h, d),
        gravity,
        {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625},
        TextureID::WINDOW
    );
    this->addChildren(body);

    // 4. THE GLASS (Inner Glowing Part)
    Object* glass = new Cube(
        Vec3<float>(0.0f, 0.0f, 0.0f),
        Vec3<float>(w - 0.04f, h - 0.04f, d - 0.04f)
    );
    this->addChildren(glass);

    config.isDirectional = false;
    config.position = Vec3<float>(0.0f, 0.0f, 0.0f);
    config.color = Vec3<float>(1.0f, 1.0f, 1.0f);
    config.spotDirection = Vec3<float>(0.0f, -1.0f, 0.0f);
    config.spotExponent = 0.01f;
    config.spotCutoff = 20.0f;
    lightID = LightingManager::createLight(config);
    LightingManager::registerLight(lightID, &config, this);
}