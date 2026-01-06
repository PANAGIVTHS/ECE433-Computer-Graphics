#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "utilities.h"

class LightingManager {
public:
    static void init();
    static void startLightSource(int id, const Vec3<float>& position, const Vec3<float>& color);
    static void endLightSource();
};

#endif