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
    Vec3<GLfloat> position, velocity;
    bool gravity = true;

    virtual void drawInternal() = 0;
public:
    Object(Vec3<GLfloat> &pos);
    Object(GLfloat x, GLfloat y, GLfloat z);
    Object(Vec3<GLfloat> &pos, bool gravity);
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity);
    ~Object();
    void draw();
    bool hasGravity();
    void setGravity(bool gravity);
    Vec3<GLfloat>& getPosition();
    void setPosition(Vec3<GLfloat>& position);
    Vec3<GLfloat>& getVelocity();
    void setVelocity(Vec3<GLfloat>& velocity);
    void applyVelocity();
};

class ObjectHandler {
private:
    static std::vector<Object *> objects;
public:
    static void addObject(Object *o);
    static void removeObject(Object *o);
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
