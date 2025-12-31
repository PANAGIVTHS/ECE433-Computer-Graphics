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
protected:
    static constexpr bool DEFAULT_GRAVITY = true;

    std::vector<Object *> children;
    Vec3<GLfloat> position, velocity;
    bool gravity, hidden = false;

    virtual void drawInternal() {};
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY);
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY) : Object(Vec3(x, y, z), gravity) {};
    virtual ~Object();
    
    void draw();
    virtual void update();
    Object *addChildren(Object *object);

    bool isHidden();
    void setHidden(bool hidden);
    bool hasGravity();
    void setGravity(bool gravity);
    Vec3<GLfloat>& getPosition();
    void setPosition(Vec3<GLfloat> position);
    Vec3<GLfloat>& getVelocity();
    void setVelocity(Vec3<GLfloat> velocity);
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
    Terrain(float x, float y, float z, bool gravity) : Object(x, y, z, gravity) {}
private:
    void drawInternal();
};

class Cube : public Object {
public: 
    Cube(float x, float y, float z) : Object(x, y, z) {}
    Cube(float x, float y, float z, bool gravity) : Object(x, y, z, gravity) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z) : Object(x, y, z) {}
    Sphere(float x, float y, float z, bool gravity) : Object(x, y, z, gravity) {}
private:
    void drawInternal();
};

#endif
