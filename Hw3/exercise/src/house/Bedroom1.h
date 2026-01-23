#ifndef SRC_BEDROOM1_H
#define SRC_BEDROOM1_H
#include "BuildingBlocks.h"
#include "House.h"

class Bedroom1 : public Block {
public:
    static inline GLfloat width = 3.61f;
    static inline GLfloat length = 2.6416f;
    static inline GLfloat backWallSideWidth = 0.8f;

    static inline GLfloat doorWidth = 0.9f;
    static inline GLfloat doorHeight = 2.0f;

    static inline GLfloat windowFrame = 0.05f;
    static inline GLfloat windowWidth = 0.4f;
    static inline GLfloat windowHeight = 0.8f;
    static inline GLfloat windowSpacing = 0.05f;
    static inline GLfloat windowHeightOffset = 1.0f;
    static inline GLint windowRows = 1;
    static inline GLint windowColumns = 2;

    static inline GLfloat wardrobeWidth = 1.8f;
    static inline GLfloat wardrobeHeight = 1.8f;
    static inline GLfloat wardrobeLength = 0.7f;
    static inline GLfloat wardrobeWallSpacing = 0.15f * wardrobeWidth;
    static inline GLfloat wardrobeDoorWidth = (wardrobeWidth - wardrobeWallSpacing * 2) * 0.5f;

    static inline GLfloat lavatoryWidth = 2.309601f;
    static inline GLfloat lavatoryLength = 1.8f;
    static inline GLfloat lavatoryWallInset = 0.1f;

    static inline GLfloat totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
    static inline GLfloat totalHeight = House::height + House::floorHeight;
    static inline GLfloat totalLength = length + House::exteriorWallThickness + House::ridgeThickness + 2 * House::interiorWallThickness + wardrobeLength;

    Bedroom1(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); Bedroom1::updateDimensions(); Bedroom1::addAll(); }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
protected:
    void addAll() override;

    std::vector<GLfloat*> getScalableVars() override {
        return {
            &width,
            &length,
            &doorWidth,
            &doorHeight,
            &windowFrame,
            &windowWidth,
            &windowHeight,
            &windowSpacing,
            &windowHeightOffset,
            &wardrobeWidth,
            &wardrobeHeight,
            &wardrobeLength,
            &backWallSideWidth,
            &lavatoryWidth,
            &lavatoryLength
        };
    }

    void updateDimensions() override {
        totalWidth = width + House::exteriorWallThickness + House::ridgeThickness;
        totalHeight = House::height + House::floorHeight;
        totalLength = length + House::exteriorWallThickness + House::ridgeThickness + 2 * House::interiorWallThickness + lavatoryWidth;
    }
private:
    Object *addFloor();
    FramedWindow *addWindows();
    Object *addWalls(FramedWindow *window, FramedWindow *door);
    FramedWindow *addDoor();
    Object *addFurniture();
};

#endif //SRC_BEDROOM1_H