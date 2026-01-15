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
#include <string>

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
    std::string to_string(int depth);
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, std::initializer_list<Object*> children = {});
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, std::initializer_list<Object*> children = {}) : Object(Vec3(x, y, z), gravity, texture) {}
    virtual ~Object();

    virtual void setTextureConfig(TextureConfig config) {}
    
    void draw();
    virtual void update();
    void optimize(bool ignorePrevious);
    Object *addChildren(Object *object);

    bool isHidden();
    void setHidden(bool hidden);
    bool hasGravity();
    void setGravity(bool gravity);
    void setTexture(TextureID id);
    Object* setScale(Vec3<GLfloat> scale);
    Object* setScale(GLfloat x, GLfloat y, GLfloat z);
    Object* setRotation(GLfloat angle, Vec3<GLfloat> axis);
    Vec3<GLfloat>& getPosition();
    void setPosition(Vec3<GLfloat> position);
    Vec3<GLfloat>& getVelocity();
    void setVelocity(Vec3<GLfloat> velocity);
    std::string to_string();
};

class ObjectHandler {
private:
    static std::vector<Object *> objects;
public:
    static void addObject(Object *o);
    static void removeObject(Object *o);
    static std::vector<Object *> &getObjects();
    static void optimizeAll(bool ignorePrevious);
    static void clear();
};

class Cuboid : public Object {
    TextureConfig texConfig;
public: 
    Cuboid(Vec3<float> pos, Vec3<float> dim, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture), texConfig(config) {
            setScale(dim); 
            this->optimize(true);
        }

    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, texture, config) {}

    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(pos, Vec3<float>(w, h, l), gravity, texture, config) {}

    Cuboid(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, texture, config) {}

    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }
private:
    void drawInternal();
};

class Cube : public Object {
public: 
    Cube(Vec3<float> pos, Vec3<float> dim, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture) {
            setScale(dim);
            this->optimize(true);
        }

    Cube(float x, float y, float z, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cube(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, texture, config) {}

    Cube(Vec3<float> pos, float w, float h, float l, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cube(pos, Vec3<float>(w, h, l), gravity, texture, config) {}

    Cube(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE, TextureConfig config = TextureConfig())
        : Cube(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, texture, config) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z, bool gravity = true, TextureID texture = TextureID::NONE)
        : Object(x, y, z, gravity, texture) { this->optimize(true); }
private:
    void drawInternal();
};


#endif
