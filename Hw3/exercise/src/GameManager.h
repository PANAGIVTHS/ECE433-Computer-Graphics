#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Camera.h"
#include "Environment.h"
#include "House.h"

class GameManager {
private:
    inline static Camera *camera = nullptr;
    inline static Environment *environment = nullptr;
    inline static House *house = nullptr;
    inline static GLint oldTime;
public:
    static constexpr GLfloat speed = 1.4f;
    static constexpr GLfloat sensitivity = 45.0f;
    static constexpr GLfloat gravity = 9.81f; // Disabled until collision detection is implemented
    static constexpr GLdouble fov = 90.0f;
    static constexpr GLdouble near = 0.1;
    static constexpr GLdouble far = 2000;
    inline static const Vec3<GLfloat> initialCameraPos = Vec3(0.0f, 0.0f, 0.0f);
    inline static const Color3f skyColor = {.red = 0.3828125f, .green = 0.75390625f, .blue = 0.89453125f};
    inline static GLdouble dt;

    static void init();
    static void onWindowUpdate(GLint width, GLint height, bool newContext);
    static void updateDeltaTime();
    static void updatePositions();
    static void cleanUp();
    
    static Camera *getCamera();
    static Environment *getEnvironment();
    static House *getHouse();
};

#endif
