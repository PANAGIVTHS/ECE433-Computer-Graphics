#ifndef SRC_BUILDINGBLOCKS_H
#define SRC_BUILDINGBLOCKS_H
#include "../Object.h"

class Block : public Object {
    Vec3<GLfloat> dim;
public:
    Block(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim) {}

    Vec3<GLfloat> getDimensions() const {
        return dim;
    }
};

class AnchoredCuboid : public Object {
protected:
    Cuboid *cuboid = nullptr;
public:
    AnchoredCuboid(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
        : Object(pos, gravity, color, texture, material) {
        cuboid = new Cuboid(dim / 2.0f, dim, gravity, color, texture, material, config, subdivisions);
        Object::addChildren(cuboid);
    }

    Vec3<GLfloat> getDimensions() const {
        return cuboid->getDimensions();
    }
};

class AnchoredCube : public Object {
protected:
    Cube *cube = nullptr;
public:
    AnchoredCube(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) {
        cube = new Cube(dim / 2.0f, dim, gravity, color, texture, material);
        Object::addChildren(cube);
    }

    Vec3<GLfloat> getDimensions() const {
        return cube->getDimensions();
    }
};

class RidgedWall : public Object {
    Vec3<GLfloat> dim;
    Vec3<GLfloat> protrusionVec;
    GLint count;
    GLfloat ridgeThickness;

    void generateStructure();
public:
    RidgedWall(Vec3<GLfloat> pos, Vec3<GLfloat> dim, Vec3<GLfloat> protrusionVec, GLint count, GLfloat ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim), protrusionVec(protrusionVec), count(count), ridgeThickness(ridgeThickness) {
        generateStructure();
    }

    Vec3<GLfloat> getDimensions() const {
        return dim;
    }
};

class GlowingCube : public Cube {
public:
    GlowingCube(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE,
        MaterialID material = DEFAULT_MATERIAL)
        : Cube(pos, dim, gravity, color, texture, material) {
    }

    void drawInternal() override;
};


class BorderCubes : public Object {
public:
    static constexpr float CUBE_SIZE = 0.1f;
    BorderCubes(Vec3<GLfloat> startPos, Vec3<GLfloat> dimensions)
        : Object(startPos, false, DEFAULT_COLOR, TextureID::NONE, MaterialID::MATTE) {
        createCorners(dimensions.x, dimensions.y, dimensions.z);
    }
    BorderCubes(Block *block) : BorderCubes(block->getPosition(), block->getDimensions()) {}
    BorderCubes(AnchoredCube *cube) : BorderCubes(cube->getPosition(), cube->getDimensions()) {}
    BorderCubes(AnchoredCuboid *cuboid) : BorderCubes(cuboid->getPosition(), cuboid->getDimensions()) {}
private:
    void createCorners(float w, float h, float l);
    void drawInternal() override {}
};

#endif //SRC_BUILDINGBLOCKS_H