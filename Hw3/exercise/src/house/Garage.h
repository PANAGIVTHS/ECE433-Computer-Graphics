#ifndef SRC_GARAGE_H
#define SRC_GARAGE_H
#include "BuildingBlocks.h"
#include "../Object.h"

class Garage : public Block {
public:
    static constexpr GLfloat floorHeight = 0.05f;
    static constexpr GLfloat wallThickness = 0.25f;

    static constexpr GLfloat length = 7.3152f;
    static constexpr GLfloat width = 1.2 * 3.8608f;
    static constexpr GLfloat height = 4.0f;
    static constexpr GLfloat ceilingOffset = 0.6f;

    static constexpr GLfloat ridgeThickness = 0.1f;
    static constexpr GLfloat doorWidth = 3.2f;
    static constexpr GLfloat doorHeight = 2.8f;
    static constexpr GLfloat sidePanelWidth = (width - doorWidth) / 2.0f;
    static constexpr GLfloat headerHeight = height - doorHeight;

    static constexpr Color3f wallColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static constexpr Color3f concreteColor = {0.55f, 0.57f, 0.58f};

    static constexpr MaterialID wallMaterial = MaterialID::MATTE;

    static constexpr GLfloat totalWidth = width + ceilingOffset + wallThickness;
    static constexpr GLfloat totalHeight = height + wallThickness;
    static constexpr GLfloat totalLength = length + 2 * ceilingOffset;

private:
    void addAll();
    Object *addFloor();
    Object *addRightSide();
    Object *addFrontSide();
    Object *addBackSide();
    Object *addCeiling();
    Object *addDoor();
public:
    Garage(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = wallColor, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Block(pos, Vec3(totalWidth, totalHeight, totalLength), gravity, color, texture, material) { addAll(); }
};

#endif //SRC_GARAGE_H