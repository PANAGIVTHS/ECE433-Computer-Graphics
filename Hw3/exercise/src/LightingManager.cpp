#include "LightingManager.h"
#include "Object.h"

std::map<int, RegisteredLight> LightingManager::registry;

void LightingManager::init() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

void LightingManager::registerLight(int id, LightConfig* config, Object* owner) {
    if (id < 0 || id >= MAX_LIGHTS) return;
    registry[id] = { config, owner };
}

void LightingManager::updateAllLights() {
    for (auto const& [id, reg] : registry) {
        if (reg.owner) {
             reg.config->position = reg.owner->getWorldPosition();
        }
        updateLight(id, *reg.config);
    }
}

int LightingManager::createLight(const LightConfig& config) {
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
    updateLight(freeID, config);

    return freeID;
}

void LightingManager::removeLight(int id) {
    if (id < 0 || id >= MAX_LIGHTS) return;

    glDisable(GL_LIGHT0 + id);
    lightAllocation[id] = false;
    registry.erase(id);
}

void LightingManager::updateLight(int id, const LightConfig& config) {
    GLenum lightID = GL_LIGHT0 + id;
    glEnable(lightID);

    GLfloat w = config.isDirectional ? 0.0f : 1.0f;
    GLfloat pos[] = { config.position.x, config.position.y, config.position.z, w };
    glLightfv(lightID, GL_POSITION, pos);

    float ambR = (config.ambient.x == -1.0f) ? config.color.x * 0.2f : config.ambient.x;
    float ambG = (config.ambient.y == -1.0f) ? config.color.y * 0.2f : config.ambient.y;
    float ambB = (config.ambient.z == -1.0f) ? config.color.z * 0.2f : config.ambient.z;

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
    if (enable) glEnable(GL_LIGHT0 + id);
    else        glDisable(GL_LIGHT0 + id);
}