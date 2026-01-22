#ifndef SRC_BATHROOM_H
#define SRC_BATHROOM_H
#include "BuildingBlocks.h"
#include "House.h"

class Bathroom : public Block {
public:
    static inline GLfloat width = 1.828f;
    static inline GLfloat length = 5.3768f;
    static inline GLfloat floorHeight = 0.27f;

    static inline GLfloat doorWidth = 0.9f;
    static inline GLfloat doorHeight = 2.0f;

    static inline GLfloat windowFrame = 0.05f;
    static inline GLfloat windowWidth = 0.8f;
    static inline GLfloat windowHeight = 0.3f;
    static inline GLfloat windowSpacing = 0.05f;
    static inline GLfloat windowHeightOffset = 2.2f;
    static inline GLint windowRows = 1;
    static inline GLint windowColumns = 1;

    static inline GLfloat leftWallSideWidth = 2.1f;
    static inline GLfloat rightWallSideWidth = 0.1f;
    static inline GLfloat rightWallInset = 0.1f;

    static inline GLfloat totalWidth = width + House::exteriorWallThickness + House::ridgeThickness + House::interiorWallThickness + rightWallInset;
    static inline GLfloat totalHeight = House::height + floorHeight;
    static inline GLfloat totalLength = length + House::interiorWallThickness;

    Bathroom(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); Bathroom::updateDimensions(); Bathroom::addAll(); }

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
            &windowFrame,
            &windowWidth,
            &windowHeight,
            &windowSpacing,
            &windowHeightOffset,
            &leftWallSideWidth,
            &rightWallSideWidth,
            &rightWallInset,
        };
    }

    void updateDimensions() override {
        totalWidth = width + House::exteriorWallThickness + House::ridgeThickness + House::interiorWallThickness + rightWallInset;
        totalHeight = House::height + floorHeight;
        totalLength = length + House::interiorWallThickness;
    }
private:
    Object *addFloor();
    FramedWindow *addWindows();
    Object *addWalls(FramedWindow *window, FramedWindow *door);
    FramedWindow *addDoor();
    Object *addFurniture();
};

#endif //SRC_BATHROOM_H