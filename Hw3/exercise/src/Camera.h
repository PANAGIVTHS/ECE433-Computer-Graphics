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
    Vec3<GLfloat> pos;
    Vec3<GLfloat> direction = Vec3<GLfloat>(0.0f, 0.0f, -1.0f);
    Vec3<GLfloat> up = Vec3<GLfloat>(0.0f, 1.0f, 0.0f);
public:
    Camera(Vec3<GLfloat> &pos): pos(pos) {}
    Camera(GLfloat x, GLfloat y, GLfloat z): pos(Vec3(x, y, z)) {}
    void offset(GLfloat x, GLfloat y, GLfloat z);
    void moveTo(GLfloat x, GLfloat y, GLfloat z);
    void move(Direction dir, GLfloat amount);
    void offsetCenter(GLfloat x, GLfloat y, GLfloat z); // TODO Change to rotateYaw and rotatePitch
    void moveCenter(GLfloat x, GLfloat y, GLfloat z); // TODO Remove when rotate is implemented
    void set();
};

#endif