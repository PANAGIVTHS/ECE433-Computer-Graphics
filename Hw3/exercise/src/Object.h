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
    static constexpr Color3f DEFAULT_COLOR = {.red = 1.0f, .green = 1.0f, .blue = 1.0f};
    static constexpr TextureID DEFAULT_TEXTURE = TextureID::NONE;
    static constexpr MaterialID DEFAULT_MATERIAL = MaterialID::NONE;

    std::vector<Object *> children;
    Object* parent = nullptr;

    Vec3<GLfloat> velocity;

    bool isStatic = false;
    Transform transform;
    GLuint displayList = 0;
    bool gravity, hidden = false;
    Color3f color;
    TextureID texture;
    MaterialID material;
    
    void applyParentTransforms();
    virtual void drawInternal() {};
    std::string toString(int depth);
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, std::initializer_list<Object*> children = {});
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, std::initializer_list<Object*> children = {})
    : Object(Vec3(x, y, z), gravity, color, texture, material, children) {}
    virtual ~Object();

    virtual void setTextureConfig(TextureConfig config) {}
    virtual Vec3<GLfloat> getWorldPosition();

    void draw();
    virtual void update();
    void optimize();
    void invalidateDisplayList();
    virtual Object *addChildren(Object *object);

    bool hasTransparency();
    bool isHidden();
    Object* setHidden(bool hidden);
    bool hasGravity();
    Object* setGravity(bool gravity);
    Object* setTexture(TextureID id, bool recurse);
    Object* setMaterial(MaterialID id, bool recurse);
    Object* setColor(Color3f color, bool recurse);
    Object* setScale(Vec3<GLfloat> scale);
    Object* setScale(GLfloat x, GLfloat y, GLfloat z);
    Object* setRotation(GLfloat angle, Vec3<GLfloat> axis);

    Vec3<GLfloat>& getPosition();
    Object* setPosition(Vec3<GLfloat> position);
    Vec3<GLfloat>& getVelocity();
    Object* setVelocity(Vec3<GLfloat> velocity);
    std::vector<Object *>& getChildren();
    std::string toString();
};

class ObjectHandler {
protected:
    static std::vector<Object *> objects;
    static std::vector<Object *> transObjects;
public:
    static void addObject(Object *o);
    static void removeObject(Object *o);
    static std::vector<Object *> &getObjects();
    static std::vector<Object *> &getTransObjects();
    static void invalidateDisplayListAll();
    static void clear();
};

class Cuboid : public Object {
    TextureConfig texConfig;
    int subdivisions;
    Vec3<GLfloat> dim;
protected:
    void drawInternal() override;
public:
    Cuboid(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
        : Object(pos, gravity, color, texture, material), texConfig(config), subdivisions(subdivisions) { this->dim = dim; setScale(dim); }


    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
            : Cuboid(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, color, texture, material, config, subdivisions) {}


    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
            : Cuboid(pos, Vec3<float>(w, h, l), gravity, color, texture, material, config, subdivisions) {}


    Cuboid(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
            : Cuboid(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, color, texture, material, config, subdivisions) {}

    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }

    void setSubdivisions(int subdivisions) {
        this->subdivisions = subdivisions;
    }

    Vec3<GLfloat> getDimensions() {
        return dim;
    }
};

class Cube : public Object {
    Vec3<GLfloat> dim;
protected:
    void drawInternal() override;
public:
    Cube(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { this->dim = dim; setScale(dim); }

    Cube(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, color, texture, material) {}

    Cube(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(pos, Vec3<float>(w, h, l), gravity, color, texture, material) {}

    Cube(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, color, texture, material) {}

    Vec3<GLfloat> getDimensions() {
        return dim;
    }
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Object(x, y, z, gravity, color, texture, material) {}
protected:
    void drawInternal() override;
};

#endif
