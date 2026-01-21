#ifndef SRC_GARAGE_H
#define SRC_GARAGE_H
#include "BuildingBlocks.h"
#include "House.h"
#include "../Object.h"

class Garage : public Block {
public:
    static inline GLfloat floorHeight = 0.05f;
    static inline GLfloat wallThickness = House::exteriorWallThickness;

    static inline GLfloat length = 7.3152f;
    static inline GLfloat width = 1.2 * 3.8608f;
    static inline GLfloat height = 4.0f;
    static inline GLfloat ceilingOffset = 0.6f;

    static inline GLfloat ridgeThickness = 0.1f;
    static inline GLfloat doorWidth = 3.2f;
    static inline GLfloat doorHeight = 2.8f;
    static inline GLfloat sidePanelWidth = (width - doorWidth) / 2.0f;
    static inline GLfloat headerHeight = height - doorHeight;

    static inline Color3f wallColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static inline Color3f concreteColor = {0.55f, 0.57f, 0.58f};

    static inline MaterialID wallMaterial = MaterialID::MATTE;

    static inline GLfloat totalWidth = width + ceilingOffset + wallThickness;
    static inline GLfloat totalHeight = height + wallThickness;
    static inline GLfloat totalLength = length + 2 * ceilingOffset;

    Garage(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = wallColor, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Block(pos, scale, gravity, color, texture, material) { performScaling(); updateDimensions(); addAll(); }
protected:
    void addAll() override;

    std::vector<GLfloat *> getScalableVars() override {
        return {
            &floorHeight,
            &wallThickness,
            &length,
            &width,
            &height,
            &ceilingOffset,
            &ridgeThickness,
            &doorWidth,
            &doorHeight,
            &sidePanelWidth,
            &headerHeight
        };
    }

    void updateDimensions() override {
        totalWidth = width + ceilingOffset + wallThickness;
        totalHeight = height + wallThickness;
        totalLength = length + 2 * ceilingOffset;
    }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
private:
    Object *addFloor();
    Object *addRightSide();
    Object *addFrontSide();
    Object *addBackSide();
    Object *addCeiling();
    Object *addDoor();
};

#endif //SRC_GARAGE_H