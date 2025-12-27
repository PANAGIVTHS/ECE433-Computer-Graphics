#ifndef OBJECT_H
#define OBJECT_H
#include "utilities.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Object {
private:
    Vec3<GLfloat> pos;

    virtual void drawInternal() = 0;
public:
    Object(Vec3<GLfloat> &pos): pos(pos) {}
    Object(GLfloat x, GLfloat y, GLfloat z): pos(Vec3(x, y, z)) {}
    void draw();
};

class Cube : public Object {
public: 
    Cube(float x, float y, float z) : Object(x, y, z) {}
private:
    void drawInternal() {
        glutSolidCube(1.0f);
    }
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z) : Object(x, y, z) {}
private:
    void drawInternal() {
        glutSolidSphere(1.0f, 20, 20);
    }
};

#endif
