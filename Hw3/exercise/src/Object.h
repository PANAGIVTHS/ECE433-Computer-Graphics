#ifndef OBJECT_H
#define OBJECT_H
#include "utilities.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include "TextureEnums.h"

class Object {
protected:
    static constexpr bool DEFAULT_GRAVITY = true;

    std::vector<Object *> children;
    Vec3<GLfloat> position, velocity;
    bool gravity, hidden = false;
    TextureID texture = TextureID::NONE;

    virtual void drawInternal() {};
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE);
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE) : Object(Vec3(x, y, z), gravity, texture) {}
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
    Terrain(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE)
        : Object(x, y, z, gravity, texture) {}
private:
    void drawInternal();
};

class Cuboid : public Object {
    float width, height, length;
    TextureConfig texConfig;
public: 
    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(x, y, z, gravity, texture), width(w), height(h), length(l), texConfig(config) {}

    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture), width(w), height(h), length(l), texConfig(config) {}

    //TODO check if directions are set correctly
    Cuboid(Vec3<float> pos, Vec3<float>dim, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture), width(dim.x), height(dim.y), length(dim.z), texConfig(config) {}

    Cuboid(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(x, y, z, gravity, texture), width(1.0f), height(1.0f), length(1.0f), texConfig(config) {}
private:
    void drawInternal();
};

class Cube : public Object {
public: 
    Cube(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE)
        : Object(x, y, z, gravity, texture) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE)
        : Object(x, y, z, gravity, texture) {}
private:
    void drawInternal();
};


#endif
