#ifndef OBJECT_H
#define OBJECT_H
#include "utilities.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

class Object {
private:
    Vec3<GLfloat> pos;

    virtual void drawInternal() = 0;
public:
    Object(Vec3<GLfloat> &pos);
    Object(GLfloat x, GLfloat y, GLfloat z);
    void draw();
};

class ObjectHandler {
private:
    static std::vector<Object *> objects;
public:
    static void addObject(Object *o);
    static std::vector<Object *> &getObjects();
    static void clear();
};

class Terrain : public Object {
public: 
    Terrain(float x, float y, float z) : Object(x, y, z) {}
private:
    void drawInternal();
};

class Cube : public Object {
public: 
    Cube(float x, float y, float z) : Object(x, y, z) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z) : Object(x, y, z) {}
private:
    void drawInternal();
};

#endif
