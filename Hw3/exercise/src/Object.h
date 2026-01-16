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
    Vec3<GLfloat> velocity;

    Transform transform;
    GLuint displayList = 0;
    bool gravity, hidden = false;
    Color3f color;
    TextureID texture;
    MaterialID material;

    virtual void drawInternal() {};
    std::string toString(int depth);
public:
    Object(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, std::initializer_list<Object*> children = {});
    Object(GLfloat x, GLfloat y, GLfloat z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, std::initializer_list<Object*> children = {})
    : Object(Vec3(x, y, z), gravity, color, texture, material, children) {}
    virtual ~Object();

    virtual void setTextureConfig(TextureConfig config) {}
    
    void draw();
    virtual void update();
    void optimize();
    void invalidateDisplayList();
    Object *addChildren(Object *object);

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
    std::string toString();
};

class ObjectHandler {
    static std::vector<Object *> objects;
public:
    static void addObject(Object *o);
    static void removeObject(Object *o);
    static std::vector<Object *> &getObjects();
    static void invalidateDisplayListAll();
    static void clear();
};

class Cuboid : public Object {
    TextureConfig texConfig;
public:
    Cuboid(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig())
        : Object(pos, gravity, color, texture, material), texConfig(config) { setScale(dim); }

    Cuboid(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, color, texture, material, config) {}

    Cuboid(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig())
        : Cuboid(pos, Vec3<float>(w, h, l), gravity, color, texture, material, config) {}

    Cuboid(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig())
        : Cuboid(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, color, texture, material, config) {}

    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }
private:
    void drawInternal();
};

class Cube : public Object {
public:
    Cube(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { setScale(dim); }

    Cube(float x, float y, float z, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(Vec3<float>(x, y, z), Vec3<float>(w, h, l), gravity, color, texture, material) {}

    Cube(Vec3<float> pos, float w, float h, float l, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(pos, Vec3<float>(w, h, l), gravity, color, texture, material) {}

    Cube(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(Vec3<float>(x, y, z), Vec3<float>(1.0f, 1.0f, 1.0f), gravity, color, texture, material) {}
private:
    void drawInternal();
};

class Sphere : public Object {
public: 
    Sphere(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) {}
private:
    void drawInternal();
};

class RidgedWall : public Object {
public:
    RidgedWall(Vec3<float> pos, Vec3<float> dim, int ridges, float ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) {
            generateStructure(dim, ridges, ridgeThickness);
    }

private:
    void generateStructure(Vec3<float> dim, int count, float ridgeThickness) {
        Cuboid* baseWall = new Cuboid(
            Vec3<float>(0.0f, 0.0f, 0.0f),
            dim, 
            gravity, 
            color,
            texture
        );
        this->addChildren(baseWall);

        float spacing = dim.x / (float)count;
        float ridgeWidth = spacing * 0.2f;
        float ridgeHeight = dim.y;
        
        float zPos = (dim.z / 2.0f) + (ridgeThickness / 2.0f);
        float startX = -dim.x / 2.0f + (spacing / 2.0f);

        for (int i = 0; i < count; i++) {
            float xPos = startX + (i * spacing);

            Cuboid* ridge = new Cuboid(
                Vec3<float>(xPos, 0.0f, zPos),
                Vec3<float>(ridgeWidth, ridgeHeight, ridgeThickness),
                gravity, 
                color,
                texture 
            );

            this->addChildren(ridge);
        }
    }
};

class Garage : public Object {
public: 
    Garage(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { addAll(); }
private:
    void addAll() {
        GLfloat wallThickness = 0.25f;
        GLfloat width = 7.3152f;
        GLfloat length = 3.8608f;
        GLfloat height = 4.0f;

        // RIGHT WALL
        GLint ridgeCount = 23;
        GLfloat ridgeThickness = 0.1f;
        Object* rightWall = new RidgedWall(
            Vec3(0.0f, 0.0f, 0.0f),
            Vec3(width, height, wallThickness),
            ridgeCount,
            ridgeThickness,
            false,
            color
        );
        rightWall->setRotation(90, Vec3(0.0f, 1.0f, 0.0f));
        addChildren(rightWall);

        // LEFT WALL
        // Object* leftWall = new RidgedWall(
        //     Vec3(-length, 0.0f, 0.0f),
        //     Vec3(width, height, wallThickness),
        //     ridgeCount,
        //     ridgeThickness,
        //     false,
        //     color
        // );
        // leftWall->setRotation(-90, Vec3(0.0f, 1.0f, 0.0f));
        // addChildren(leftWall);

        // BACK WALL
        ridgeCount = 9;
        ridgeThickness = 0.1f;
        Object* backWall = new RidgedWall(
            Vec3(-(length+wallThickness)/2, 0.0f, -(width - wallThickness)/2),
            Vec3(length, height, wallThickness),
            ridgeCount,
            ridgeThickness,
            false,
            color
        );
        backWall->setRotation(180, Vec3(0.0f, 1.0f, 0.0f));
        addChildren(backWall);

        // FRONT WALL
        Object* frontWall = new RidgedWall(
            Vec3(-(length+wallThickness)/2, 0.0f, (width - wallThickness)/2),
            Vec3(length, height, wallThickness),
            ridgeCount,
            ridgeThickness,
            false,
            color
        );
        frontWall->setRotation(0, Vec3(0.0f, 1.0f, 0.0f));
        addChildren(frontWall);

        // CEILING
        GLfloat ceilingOffset = 0.6f;
        GLfloat ceilingWidth = width + 2 * ceilingOffset;
        GLfloat ceilingHeight = length + wallThickness + ceilingOffset;
        Object* ceiling = new Cube(
            Vec3(-(length - ceilingOffset)/2.0f, (height + wallThickness) / 2.0f, 0.0f),
            Vec3(ceilingHeight, wallThickness, ceilingWidth),
            false,
            color
        );
        addChildren(ceiling);
        
    }
    
    void drawInternal() override {}
};

#endif
