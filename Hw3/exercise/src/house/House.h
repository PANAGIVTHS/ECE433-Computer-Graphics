#ifndef SRC_HOUSE_H
#define SRC_HOUSE_H
#include "../Object.h"

class House : public Object {
    void setup();
public:
    House(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { setup(); }
};

class RidgedWall : public Object {
public:
    RidgedWall(Vec3<float> pos, Vec3<float> dim, int ridges, float ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) {
        generateStructure(dim, ridges, ridgeThickness);
    }
private:
    void generateStructure(Vec3<float> dim, int count, float ridgeThickness) {
        Cube* baseWall = new Cube(
            Vec3<float>(0.0f, 0.0f, 0.0f),
            dim,
            gravity,
            color,
            texture,
            material
        );
        this->addChildren(baseWall);

        float spacing = dim.x / (float)count;
        float ridgeWidth = spacing * 0.2f;
        float ridgeHeight = dim.y;

        float zPos = (dim.z / 2.0f) + (ridgeThickness / 2.0f);
        float startX = -dim.x / 2.0f + (spacing / 2.0f);

        for (int i = 0; i < count; i++) {
            float xPos = startX + (i * spacing);

            Cube* ridge = new Cube(
                Vec3<float>(xPos, 0.0f, zPos),
                Vec3<float>(ridgeWidth, ridgeHeight, ridgeThickness),
                gravity,
                color,
                texture,
                material
            );

            this->addChildren(ridge);
        }
    }
};


class GlowingCube : public Cube {
public:
    GlowingCube(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE,
        MaterialID material = DEFAULT_MATERIAL)
        : Cube(pos, dim, gravity, color, texture, material) {
    }

    void drawInternal() override {
        glDisable(GL_LIGHTING);
        Cube::drawInternal();
        glEnable(GL_LIGHTING);
    }
};

#endif //SRC_HOUSE_H