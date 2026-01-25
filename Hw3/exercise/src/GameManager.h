#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Camera.h"
#include "Environment.h"
#include "Object.h"
#include "house/House.h"

class GameManager {
private:
    inline static Camera *camera = nullptr;
    inline static Environment *environment = nullptr;
    inline static House *house = nullptr;
    inline static GLint oldTime;

    //fps related
    inline static int frameCount = 0;
    inline static int lastFpsTime = 0;
    inline static float fps = 0.0f;
    inline static std::vector<Object*> levelAssets;
    static void unloadLevelAssets();
    static void loadLevelAssets();
    static void updateDeltaTime();
public:
    static constexpr GLfloat speed = 4.137f; // m/s
    static constexpr GLfloat sensitivity = 0.03f; // deg / pixel
    static constexpr GLfloat gravity = 9.81f; // m/s^2
    static constexpr GLfloat jumpSpeed = 4.0f; // m/s
    static constexpr GLfloat maxPitch = 89.5;
    static constexpr GLdouble fov = 80.0f;
    static constexpr GLdouble near = 0.1;
    static constexpr GLdouble far = 2000;
    inline static const Vec3<GLfloat> initialCameraPos = Vec3(0.0f, 0.0f, 0.0f);
    inline static const Color3f skyColor = {.red = 0.3828125f, .green = 0.75390625f, .blue = 0.89453125f};
    inline static GLdouble dt;

    static void init();
    static void onWindowUpdate(GLint width, GLint height, bool newContext);
    static void runGameLoop();
    static void cleanUp();
    static void reloadAssets();

    //fps related
    static void updateFPS();
    static float getFPS();
    
    static Camera *getCamera();
    static Environment *getEnvironment();
};

#endif
