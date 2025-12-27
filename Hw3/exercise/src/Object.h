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
public:
    Object(Vec3<GLfloat> &pos): pos(pos) {}
    Object(GLfloat x, GLfloat y, GLfloat z): pos(Vec3(x, y, z)) {}
    void draw();
};

#endif
