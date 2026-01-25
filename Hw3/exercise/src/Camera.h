#ifndef CAMERA_H
#define CAMERA_H
#include "utilities.h"
#include "LightingManager.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Camera {
private:
    Vec3<GLfloat> position, velocity;
    Vec3<GLfloat> direction = Vec3<GLfloat>(0.0f, 0.0f, -1.0f);
    Vec3<GLfloat> up = Vec3<GLfloat>(0.0f, 1.0f, 0.0f);
    GLfloat yaw = -90.0f, pitch = 0.0f;
    bool gravity = false;
    int flashlightID = -1;
    LightConfig flashlightConfig;

    void updateDirection();
public:
    Camera(const Vec3<GLfloat> &position) : Camera(position, false) {}
    Camera(GLfloat x, GLfloat y, GLfloat z) : Camera(Vec3(x, y, z), false) {}
    Camera(GLfloat x, GLfloat y, GLfloat z, bool gravity): Camera(Vec3(x, y, z), gravity) {}
    Camera(const Vec3<GLfloat> &position, bool gravity): position(position), gravity(gravity) {
        flashlightConfig.position = Vec3<float>(0.0f, 0.0f, 0.0f);
        flashlightConfig.color    = Vec3<float>(1.0f, 1.0f, 0.8f);
        // flashlightConfig.spotCutoff = 20.0f;
        // flashlightConfig.spotExponent = 15.0f;
        // flashlightConfig.spotDirection = Vec3<float>(0.0f, 0.0f, -1.0f);
        flashlightConfig.constant = 1.0f;
        flashlightConfig.linear = 0.04f;
        flashlightConfig.quadratic = 0.0f;

        flashlightID = LightingManager::createLight(flashlightConfig);
    }
    void offset(GLfloat x, GLfloat y, GLfloat z);
    void moveTo(GLfloat x, GLfloat y, GLfloat z);
    void move(Direction dir, GLfloat amount);
    void rotateYaw(GLfloat angle);
    void rotatePitch(GLfloat angle);
    void set();
    bool hasGravity();
    void setGravity(bool gravity);
    int getFlashlightID();
    LightConfig getFlashlightConfig();
    Vec3<GLfloat> getDirection();
    Vec3<GLfloat> getPosition();
    Vec3<GLfloat>& getVelocity();
    void setVelocity(Vec3<GLfloat> velocity);
    void update();
};

#endif