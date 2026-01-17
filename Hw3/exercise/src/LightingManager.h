#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "utilities.h"
#include <map>

class Object;

struct LightConfig {
    Vec3<float> position = {0.0f, 0.0f, 0.0f};
    Vec3<float> color    = {1.0f, 1.0f, 1.0f};
    bool isDirectional   = false;    
    Vec3<float> ambient  = { -1.0f, -1.0f, -1.0f };
    Vec3<float> diffuse  = { -1.0f, -1.0f, -1.0f };
    Vec3<float> specular = { 1.0f, 1.0f, 1.0f };

    float constant  = 1.0f;
    float linear    = 0.0f;
    float quadratic = 0.0f;

    float spotCutoff = 180.0f;
    float spotExponent = 0.0f;
    Vec3<float> spotDirection = {0.0f, -1.0f, 0.0f};
};

struct RegisteredLight {
    LightConfig* config;
    Object* owner;
};

class LightingManager {
private:
    static const int MAX_LIGHTS = 8;
    static inline bool lightAllocation[8] = { false };
    static std::map<int, RegisteredLight> registry;

public:
    static void init();
    static int createLight(const LightConfig& config);
    static void updateLight(int id, const LightConfig& config);
    static void removeLight(int id);
    static void registerLight(int id, LightConfig* config, Object* owner);
    static void updateAllLights();
    void toggleLight(int id, bool enable);
};

#endif