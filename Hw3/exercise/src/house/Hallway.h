#ifndef SRC_HALLWAY_H
#define SRC_HALLWAY_H
#include "BuildingBlocks.h"
#include "DiningRoom.h"
#include "FamilyRoom.h"
#include "House.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
class Hallway : public Block {
public:
    static inline GLfloat width = 1.0f;
    static inline GLfloat length = DiningRoom::length - FamilyRoom::length - House::interiorWallThickness;

    static inline GLfloat doorHeight = 2.0f;
    static inline GLfloat doorWallInset = 0.1f;
    static inline GLfloat doorWidth = length * 0.4f;
    static inline GLfloat doorWallWidth = length * 0.1f;

    static inline GLfloat totalWidth = width + 2 * House::interiorWallThickness + doorWallInset;
    static inline GLfloat totalHeight = House::height + House::floorHeight;
    static inline GLfloat totalLength = length + 2 * House::interiorWallThickness;

    Hallway(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); Hallway::updateDimensions(); Hallway::addAll(); }

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
            &doorWallWidth,
            &doorWallInset
        };
    }

    void updateDimensions() override {
        totalWidth = width + 2 * House::interiorWallThickness + doorWallInset;
        totalHeight = House::height + House::floorHeight;
        totalLength = length + 2 * House::interiorWallThickness;
    }
private:
    Object *addFloor();
    Object *addWalls(FramedWindow *door);
    FramedWindow *addDoor();
    Object *addFurniture();
};

#endif //SRC_HALLWAY_H