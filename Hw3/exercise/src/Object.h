#ifndef OBJECT_H
#define OBJECT_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include "utilities.h"
#include "TextureEnums.h"

class Object {
protected:
    static constexpr bool DEFAULT_GRAVITY = true;

    std::vector<Object *> children;
    Vec3<GLfloat> velocity;

    Transform transform;
    GLuint displayList = 0;
    bool gravity, hidden = false;
    TextureID texture = TextureID::NONE;

    virtual void drawInternal() {};
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, std::initializer_list<Object*> children = {});
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, std::initializer_list<Object*> children = {}) : Object(Vec3(x, y, z), gravity, texture) {}
    virtual ~Object();

    virtual void setTextureConfig(TextureConfig config) {}
    
    void draw();
    virtual void update();
    Object *addChildren(Object *object);

    bool isHidden();
    void setHidden(bool hidden);
    bool hasGravity();
    void setGravity(bool gravity);
    void setTexture(TextureID id);
    void optimize();
    void invalidateDisplayList();
    Object* setScale(Vec3<GLfloat> scale);
    Object* setScale(GLfloat x, GLfloat y, GLfloat z);
    Object* setRotation(GLfloat angle, Vec3<GLfloat> axis);
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
    static void invalidateListAll();
    static void clear();
};

class Cuboid : public Object {
    TextureConfig texConfig;
public: 
    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(x, y, z, gravity, texture), texConfig(config) {
            setScale(w, h, l);
        }
    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture), texConfig(config) {
            setScale(w, h, l);
        }
    Cuboid(Vec3<float> pos, Vec3<float>dim, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture), texConfig(config) {
            setScale(dim);
        }
    Cuboid(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(x, y, z, gravity, texture), texConfig(config) {
            setScale(1.0f, 1.0f, 1.0f);
        }
    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }
private:
    void drawInternal();
};

class Cube : public Object {
public: 
    Cube(Vec3<GLfloat> pos, Vec3<GLfloat> dim, Vec3<GLfloat> rotateAxis, float angle, bool gravity = true, TextureID texture = TextureID::NONE);
    Cube(Vec3<GLfloat> pos, bool gravity, TextureID texture)
        : Object(pos, gravity, texture) {}
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
