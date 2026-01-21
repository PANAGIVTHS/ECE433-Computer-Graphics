#ifndef SRC_PORCH_H
#define SRC_PORCH_H
#include "BuildingBlocks.h"
#include "../Object.h"

class Porch : public Block {
public:
    static constexpr GLfloat frontWidth = 5.9182f;
    static constexpr GLfloat frontLength = 2.33675f;
    static constexpr GLfloat leftLength = 3.33528f;

    static constexpr GLfloat ceilingSpacing = 0.3;
    static constexpr GLfloat ceilingHeight = 0.25f;

    static constexpr GLfloat firstLayerHeight = 0.1f;
    static constexpr GLfloat secondLayerHeight = 0.17f;
    static constexpr GLfloat secondLayerSpacing = 0.1f;

    static constexpr GLfloat pillarHeight = 4 - firstLayerHeight - secondLayerHeight - ceilingHeight;
    static constexpr GLfloat pillarWidth = 0.2f;
    static constexpr GLfloat pillarInset = 0.05f;

    static constexpr Color3f floorColor = {.red = 1, .green = 1, .blue = 1};
    static constexpr MaterialID floorMaterial = MaterialID::GLOSSY;
    static constexpr Color3f ceilingColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static constexpr MaterialID ceilingMaterial = MaterialID::MATTE;

    static constexpr GLfloat totalWidth = frontWidth + 2 * (ceilingSpacing + secondLayerSpacing);
    static constexpr GLfloat totalHeight = firstLayerHeight + secondLayerHeight + pillarHeight + ceilingHeight;
    static constexpr GLfloat totalLength = frontLength + leftLength + 2 * (secondLayerSpacing + ceilingSpacing);

private:
    Object *addFloor();
    Object *addCeiling();
    Object *addPillars();

    void addAll();
    void drawInternal() override {}
public:
    Porch(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Block(pos, Vec3<GLfloat>(totalWidth, totalHeight, totalLength), gravity, color, texture, material) { addAll(); }
};

#endif //SRC_PORCH_H