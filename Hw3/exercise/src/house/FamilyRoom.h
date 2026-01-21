#ifndef SRC_FAMILYROOM_H
#define SRC_FAMILYROOM_H
#include "BuildingBlocks.h"
#include "House.h"

class FamilyRoom : public Block {
public:
    static inline GLfloat width = 396.24f;
    static inline GLfloat length = 335.28f;
    static inline GLfloat floorHeight = 0.05f;

    static inline GLfloat doorWidth = 1.2f;
    static inline GLfloat doorHeight = 2.0f;

    static inline GLfloat totalWidth = width + House::exteriorWallThickness;
    static inline GLfloat totalHeight = House::height;
    static inline GLfloat totalLength = length + House::exteriorWallThickness;

    FamilyRoom(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Block(pos, scale, gravity, color, texture, material) { performScaling(); FamilyRoom::updateDimensions(); FamilyRoom::addAll(); }
protected:
    void addAll() override;

    std::vector<GLfloat*> getScalableVars() override {
        return {
            &width,
            &length,
            &floorHeight,
            &doorWidth,
            &doorHeight
        };
    }

    void updateDimensions() override {
        totalWidth = width + House::exteriorWallThickness;
        totalHeight = House::height;
        totalLength = length + House::exteriorWallThickness;
    }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
};

#endif //SRC_FAMILYROOM_H