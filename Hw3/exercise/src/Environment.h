#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "utilities.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Environment
{
private:
    Color3f skyColor;
public:
    Environment(Color3f skyColor);
    void spawn();
};

#endif
