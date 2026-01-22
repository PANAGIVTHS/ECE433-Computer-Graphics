#ifndef SRC_FAMILYROOM_H
#define SRC_FAMILYROOM_H
#include "BuildingBlocks.h"
#include "House.h"

class FamilyRoom : public Block {
public:
    static inline GLfloat width = 4.7624f;
    static inline GLfloat length = 4.5528f;
    static inline GLfloat floorHeight = 0.27f;

    static inline GLfloat doorWidth = 1.2f;
    static inline GLfloat doorHeight = 2.0f;
    static inline GLfloat doorFrame = 0.1f;
    static inline GLfloat windowSpacing = 0.05f;
    static inline GLfloat windowWidth = 0.22f;
    static inline GLfloat windowHeight = 0.22f;
    static inline GLint windowRows = 6;
    static inline GLint windowColumns = 2;
    static inline GLint windowCount = 3;

    static inline GLfloat frontWallSideWidth = 0.2f;

    static inline GLfloat totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
    static inline GLfloat totalHeight = House::height + floorHeight;
    static inline GLfloat totalLength = length + House::exteriorWallThickness + House::ridgeThickness;

    FamilyRoom(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); FamilyRoom::updateDimensions(); FamilyRoom::addAll(); }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
protected:
    void addAll() override;

    std::vector<GLfloat*> getScalableVars() override {
        return {
            &width,
            &length,
            &floorHeight,
            &doorWidth,
            &doorHeight,
            &doorFrame,
            &windowSpacing,
            &windowWidth,
            &windowHeight,
            &frontWallSideWidth
        };
    }

    void updateDimensions() override {
        totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
        totalHeight = House::height + floorHeight;
        totalLength = length + House::exteriorWallThickness + House::ridgeThickness;
    }
private:
    Object *addFloor();
    Object *addWalls(FramedWindow *window);
    Object *addWindows();
    Object *addDoor(FramedWindow *window);
    Object *addFurniture();
};

#endif //SRC_FAMILYROOM_H