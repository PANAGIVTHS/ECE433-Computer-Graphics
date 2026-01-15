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
#include "MaterialManager.h"
#include <string>

class Object {
protected:
    static constexpr bool DEFAULT_GRAVITY = false;

    std::vector<Object *> children;
    Vec3<GLfloat> velocity;

    Transform transform;
    GLuint displayList = 0;
    bool gravity, hidden = false;
    TextureID texture = TextureID::NONE;
    MaterialID material = MaterialID::NONE;

    virtual void drawInternal() {};
    std::string toString(int depth);
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, std::initializer_list<Object*> children = {});
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, std::initializer_list<Object*> children = {}) : Object(Vec3(x, y, z), gravity, texture) {}
    virtual ~Object();

    virtual void setTextureConfig(TextureConfig config) {}
    
    void draw();
    virtual void update();
    void resetDisplayList(bool deleteOld);
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
    std::string toString();
};

class ObjectHandler {
private:
    static std::vector<Object *> objects;
public:
    static void addObject(Object *o);
    static void removeObject(Object *o);
    static std::vector<Object *> &getObjects();
    static void resetDisplayListAll(bool deleteOld);
    static void clear();
};

class Cuboid : public Object {
    TextureConfig texConfig;
public: 
    Cuboid(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture, material), texConfig(config) { setScale(dim); }

    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, texture, material, config) {}

    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(pos, Vec3<float>(w, h, l), gravity, texture, material, config) {}

    Cuboid(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, texture, material, config) {}

    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }
private:
    void drawInternal();
};

class Cube : public Object {
public:
    Cube(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Object(pos, gravity, texture, material) { setScale(dim); }

    Cube(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cube(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, texture, material, config) {}

    Cube(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cube(pos, Vec3<float>(w, h, l), gravity, texture, material, config) {}

    Cube(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE, TextureConfig config = TextureConfig())
        : Cube(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, texture, material, config) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE)
        : Object(x, y, z, gravity, texture, material) {}
private:
    void drawInternal();
};

class Garage : public Object {
public: 
    Garage(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE)
        : Object(x, y, z, gravity, texture, material) { addAll(); }
private:
    void addAll() {
        GLfloat wallThickness = 0.25f;
        GLfloat width = 7.3152f;
        GLfloat length = 3.8608f;
        GLfloat height = 4.0f;
        
        // RIGHT WALL
        addChildren(new Cube(0.0f, 0.0f, 0.0f, wallThickness, height, width));
        addChildren((new Cube(-(length+wallThickness)/2, 0.0f, -(width - wallThickness)/2, wallThickness, height, length))
                    ->setRotation(90, Vec3(0.0f, 1.0f, 0.0f)));

        // BACK WALL

        // FRONT WALL
    }
    
    void drawInternal() {}
};

#endif
