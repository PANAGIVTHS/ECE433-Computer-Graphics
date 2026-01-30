#include "LightingManager.h"
#include "WindowManager.h"
#include "Object.h"

std::map<int, RegisteredLight> LightingManager::registry;

void LightingManager::init() {
    if (!WindowManager::getMode()) return;

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    GLfloat globalAmbient[] = {0.01f, 0.01f, 0.01f, 0.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void LightingManager::updateAllLights() {
    for (auto& [id, reg] : registry) {
        if (reg.owner) {
            if (reg.owner->isHidden()) {
                glDisable(GL_LIGHT0 + id);
                continue;
            } else {
                glEnable(GL_LIGHT0 + id);
            }
        }
        
        updateLight(id);
    }
}

int LightingManager::registerLight(const LightConfig config) {
    return registerLight(config, nullptr);
}

int LightingManager::registerLight(const LightConfig config, Object *owner) {
    if (!WindowManager::getMode()) return -1;

    int freeID = -1;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (!lightAllocation[i]) {
            freeID = i;
            break;
        }
    }

    if (freeID == -1) {
        return -1;
    }

    lightAllocation[freeID] = true;
    registry[freeID] = { config, owner };

    return freeID;
}

LightConfig& LightingManager::getConfig(int id) {
    static LightConfig errorConfig;
    errorConfig.ambient = Vec3<GLfloat>(-1, -1, -1);
    if (id < 0 || id >= MAX_LIGHTS || lightAllocation[id] == false) return errorConfig;

    return registry[id].config;
} 

void LightingManager::setConfig(int id, const LightConfig config) {
    if (id < 0 || id >= MAX_LIGHTS || lightAllocation[id] == false) return;

    registry[id].config = config;
} 

void LightingManager::setOwner(int id, Object *owner) {
    if (id < 0 || id >= MAX_LIGHTS || lightAllocation[id] == false) return;

    registry[id].owner = owner;
} 

void LightingManager::removeLight(int id) {
    if (id < 0 || id >= MAX_LIGHTS || lightAllocation[id] == false) return;

    glDisable(GL_LIGHT0 + id);
    lightAllocation[id] = false;
    registry.erase(id);
}

void LightingManager::updateLight(int id) {
    if (!WindowManager::getMode()) return;

    GLenum lightID = GL_LIGHT0 + id;
    glEnable(lightID);

    LightConfig config = registry[id].config;
    Vec3<GLfloat> finalPos = registry[id].owner ? config.position + registry[id].owner->getWorldPosition() : config.position; 

    GLfloat w = config.isDirectional ? 0.0f : 1.0f;
    GLfloat pos[] = { finalPos.x, finalPos.y, finalPos.z, w };
    glLightfv(lightID, GL_POSITION, pos);

    float ambR = (config.ambient.x == -1.0f) ? config.color.x * 0.4f : config.ambient.x;
    float ambG = (config.ambient.y == -1.0f) ? config.color.y * 0.4f : config.ambient.y;
    float ambB = (config.ambient.z == -1.0f) ? config.color.z * 0.4f : config.ambient.z;

    float diffR = (config.diffuse.x == -1.0f) ? config.color.x : config.diffuse.x;
    float diffG = (config.diffuse.y == -1.0f) ? config.color.y : config.diffuse.y;
    float diffB = (config.diffuse.z == -1.0f) ? config.color.z : config.diffuse.z;

    GLfloat amb[]  = { ambR, ambG, ambB, 1.0f };
    GLfloat diff[] = { diffR, diffG, diffB, 1.0f };
    GLfloat spec[] = { config.specular.x, config.specular.y, config.specular.z, 1.0f };

    glLightfv(lightID, GL_AMBIENT,  amb);
    glLightfv(lightID, GL_DIFFUSE,  diff);
    glLightfv(lightID, GL_SPECULAR, spec);

    glLightf(lightID, GL_CONSTANT_ATTENUATION,  config.constant);
    glLightf(lightID, GL_LINEAR_ATTENUATION,    config.linear);
    glLightf(lightID, GL_QUADRATIC_ATTENUATION, config.quadratic);

    glLightf(lightID, GL_SPOT_CUTOFF, config.spotCutoff);
    if (config.spotCutoff != 180.0f) {
        GLfloat dir[] = { config.spotDirection.x, config.spotDirection.y, config.spotDirection.z };
        glLightfv(lightID, GL_SPOT_DIRECTION, dir);
        glLightf(lightID, GL_SPOT_EXPONENT, config.spotExponent);
    }
}

void LightingManager::toggleLight(int id, bool enable) {
    if (!WindowManager::getMode()) return;

    if (enable) glEnable(GL_LIGHT0 + id);
    else        glDisable(GL_LIGHT0 + id);
}