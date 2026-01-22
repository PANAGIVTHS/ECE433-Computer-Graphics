#ifndef SRC_DININGROOM_H
#define SRC_DININGROOM_H
#include "BuildingBlocks.h"
#include "House.h"

class DiningRoom : public Block {
public:
    static inline GLfloat width = 2.5908f;
    static inline GLfloat length = 7.62f;
    static inline GLfloat floorHeight = 0.27f;

    static inline GLfloat doorWidth = 1.2f;
    static inline GLfloat doorHeight = 2.0f;
    static inline GLfloat doorFrame = 0.1f;

    static inline GLfloat windowWidth = 0.23f;
    static inline GLfloat windowHeight = 0.23f;
    static inline GLfloat windowSpacing = 0.05f;
    static inline GLfloat windowHeightOffset = 0.6f;
    static inline GLint windowRows = 5;
    static inline GLint windowColumns = 2;

    static inline GLfloat frontWallSideWidth = 0.4f;
    static inline GLfloat rightWallSideWidth = 2.8f;

    static inline GLfloat totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
    static inline GLfloat totalHeight = House::height + floorHeight;
    static inline GLfloat totalLength = length + House::exteriorWallThickness + House::ridgeThickness;

    DiningRoom(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); DiningRoom::updateDimensions(); DiningRoom::addAll(); }

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
            &windowWidth,
            &windowHeight,
            &windowSpacing,
            &windowHeightOffset,
            &frontWallSideWidth,
            &rightWallSideWidth
        };
    }

    void updateDimensions() override {
        totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
        totalHeight = House::height + floorHeight;
        totalLength = length + House::exteriorWallThickness + House::ridgeThickness;
    }
private:
    Object *addFloor();
    Object *addWalls(FramedWindow *window, FramedWindow *door);
    FramedWindow *addWindows();
    FramedWindow *addDoor();
    Object *addFurniture();
};

#endif //SRC_DININGROOM_H