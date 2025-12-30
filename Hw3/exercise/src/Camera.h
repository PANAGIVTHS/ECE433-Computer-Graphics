#ifndef CAMERA_H
#define CAMERA_H
#include "utilities.h"
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

    void updateDirection();
public:
    Camera(const Vec3<GLfloat> &position): position(position) {}
    Camera(GLfloat x, GLfloat y, GLfloat z): position(Vec3(x, y, z)) {}
    Camera(const Vec3<GLfloat> &position, bool gravity): position(position), gravity(gravity) {}
    Camera(GLfloat x, GLfloat y, GLfloat z, bool gravity): position(Vec3(x, y, z)), gravity(gravity) {}
    void offset(GLfloat x, GLfloat y, GLfloat z);
    void moveTo(GLfloat x, GLfloat y, GLfloat z);
    void move(Direction dir, GLfloat amount);
    void rotateYaw(GLfloat angle);
    void rotatePitch(GLfloat angle);
    void set();
    bool hasGravity();
    void setGravity(bool gravity);
    Vec3<GLfloat> getPosition();
    Vec3<GLfloat>& getVelocity();
    void setVelocity(Vec3<GLfloat>& velocity);
    void applyVelocity();
};

#endif