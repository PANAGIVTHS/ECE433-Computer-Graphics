#include "Hallway.h"

#include "Bathroom.h"

void Hallway::addAll() {
    addChildren(addFloor());
    FramedWindow *door = addDoor();
    addChildren(door);
    addChildren(addWalls(door));
    addChildren(addFurniture());
}

Object *Hallway::addFloor() {
    Object* floors = new Object(Vec3<GLfloat>(0,0,0));

    // MAIN FLOOR
    floors->addChildren(new AnchoredCuboid(
        Vec3<GLfloat>(2 * House::interiorWallThickness, 0.0f, House::interiorWallThickness),
        Vec3<GLfloat>(width, House::floorHeight, length),
        gravity,
        House::floorColor,
        TextureID::NONE,
        MaterialID::MATTE
    ));

    GLfloat hallWidth = FamilyRoom::width - (Bathroom::width + width + 3 * House::interiorWallThickness);
    floors->addChildren(new AnchoredCuboid(
        Vec3<GLfloat>(-hallWidth + House::interiorWallThickness, 0.0f, totalLength - Bathroom::totalLength),
        Vec3<GLfloat>(hallWidth, House::floorHeight, Bathroom::totalLength),
        gravity,
        House::floorColor,
        TextureID::NONE,
        MaterialID::MATTE
    ));

    return floors;
}

Object *Hallway::addWalls(FramedWindow *door) {
    Object* walls = new Object(Vec3<GLfloat>(doorWallInset + House::interiorWallThickness,0,0));

    // LEFT WALL
    Object *leftWall = new Object(Vec3<GLfloat>(- 2 * House::interiorWallThickness + doorWallInset, 0, 0));
    leftWall->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, 0.0f),
    Vec3(House::interiorWallThickness, House::height + House::floorHeight,  House::interiorWallThickness + doorWallWidth)
    ));

    leftWall->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, House::interiorWallThickness + doorWallWidth + 2 * doorWidth),
    Vec3(House::interiorWallThickness, House::height + House::floorHeight, House::interiorWallThickness + doorWallWidth)
    ));

    leftWall->addChildren(new AnchoredCube(
        Vec3(0.0f, House::floorHeight + doorHeight, House::interiorWallThickness + doorWallWidth),
    Vec3(House::interiorWallThickness, totalHeight - (House::floorHeight + doorHeight), doorWidth * 2)
    ));

    leftWall->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, House::interiorWallThickness + doorWallWidth),
    Vec3(House::interiorWallThickness, House::floorHeight, doorWidth * 2),
        gravity,
        House::floorColor
    ));

    walls->addChildren(leftWall);

    // FRONT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(-doorWallInset - House::interiorWallThickness, 0.0f, length + House::interiorWallThickness),
    Vec3(width + House::interiorWallThickness + doorWallInset, House::height + House::floorHeight, House::interiorWallThickness)
    ));

    // RIGHT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(width, 0.0f, 0.0f),
    Vec3(House::interiorWallThickness, House::height + House::floorHeight, length + 2 * House::interiorWallThickness)
    ));

    // BACK WALL
    walls->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, 0.0f),
    Vec3(width + House::interiorWallThickness - doorWallInset, House::height + House::floorHeight, House::interiorWallThickness)
    ));

    return walls;
}

FramedWindow *Hallway::addDoor() {
    return nullptr;
}

Object *Hallway::addFurniture() {
    return nullptr;
}