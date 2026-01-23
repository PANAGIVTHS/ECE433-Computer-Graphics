#include "Bedroom1.h"

#include "Bathroom.h"
#include "Bedroom2.h"
#include "DiningRoom.h"

void Bedroom1::addAll() {
    addChildren(addFloor());
    FramedWindow *window = addWindows();
    addChildren(window);
    FramedWindow *door = addDoor();
    addChildren(door);
    addChildren(addWalls(window, door));
    addChildren(addFurniture());
}

Object *Bedroom1::addFloor() {
    Object* floors = new Object(Vec3<GLfloat>(0,0,0));

    AnchoredCuboid *mainFloor = new AnchoredCuboid(
        Vec3<GLfloat>(0.0f, 0.0f, House::ridgeThickness + House::exteriorWallThickness),
        Vec3<GLfloat>(width, House::floorHeight, length + House::interiorWallThickness),
        gravity,
        House::floorColor,
        TextureID::NONE,
        MaterialID::MATTE
    );
    floors->addChildren(mainFloor);

    GLfloat zOff = (House::interiorWallThickness + 2 * (Bedroom2::wardrobeWidth + Bedroom2::rightWallSideWidth));
    GLfloat entranceLength = length + + House::interiorWallThickness - zOff;
    AnchoredCuboid *entranceFloor = new AnchoredCuboid(
        Vec3<GLfloat>(-House::interiorWallThickness - Bedroom2::wardrobeLength, 0.0f, House::ridgeThickness + House::exteriorWallThickness + zOff),
        Vec3<GLfloat>(House::interiorWallThickness + Bedroom2::wardrobeLength, House::floorHeight, entranceLength),
        gravity,
        House::floorColor,
        TextureID::NONE,
        MaterialID::MATTE
    );
    floors->addChildren(entranceFloor);

    return floors;
}

FramedWindow *Bedroom1::addWindows() {
    FramedWindow *window = new FramedWindow(
        Vec3<GLfloat>(backWallSideWidth, windowHeightOffset + House::floorHeight, House::exteriorWallThickness * 0.5f),
        windowRows,
        windowColumns,
        windowWidth,
        windowHeight,
        windowFrame,
        windowSpacing,
        scale,
        gravity,
        House::darkColor,
        texture,
        material
    );

    return window;
}

Object *Bedroom1::addWalls(FramedWindow *window, FramedWindow *door) {
    Object* walls = new Object(Vec3<GLfloat>(0,0,0));

    // RIGHT WALL
    GLint ridgeCount = 7;
    walls->addChildren(new RidgedWall(Vec3(width, 0.0f, House::exteriorWallThickness + House::ridgeThickness),
    Vec3(House::exteriorWallThickness, House::height + House::floorHeight, 2 * House::interiorWallThickness + length + lavatoryWidth),
        Vec3(1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    // FRONT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(totalWidth - House::exteriorWallThickness - House::ridgeThickness - DiningRoom::width, 0.0f, totalLength - House::interiorWallThickness),
    Vec3(DiningRoom::width, House::height + House::floorHeight, House::interiorWallThickness)
    ));

    // BACK WALL
    Object* backWalls = new Object(Vec3<GLfloat>(0.0f,0,House::ridgeThickness));

    ridgeCount = 2;
    backWalls->addChildren(new RidgedWall(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(backWallSideWidth, House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    ridgeCount = 4;
    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth, 0.0f, 0.0f),
    Vec3(window->getDimensions().x, House::floorHeight + windowHeightOffset, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth, House::floorHeight + windowHeightOffset + window->getDimensions().y, 0.0f),
    Vec3(window->getDimensions().x, totalHeight - (House::floorHeight + windowHeightOffset + window->getDimensions().y), House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    ridgeCount = 7;
    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth + window->getDimensions().x, 0.0f, 0.0f),
    Vec3(totalWidth - (House::ridgeThickness + backWallSideWidth + window->getDimensions().x), House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    walls->addChildren(backWalls);

    // WARDROBE
    Object* wardrobe = new Object(Vec3<GLfloat>(width - wardrobeWidth, 0.0f, totalLength - wardrobeLength - House::interiorWallThickness));

    wardrobe->addChildren(new AnchoredCube(Vec3(0.0f, 0.0f, -House::interiorWallThickness),
    Vec3(House::interiorWallThickness, House::floorHeight + House::height, wardrobeLength + House::interiorWallThickness)
    ));

    // FLOOR
    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, 0.0f, 0.0f),
    Vec3(wardrobeWidth, House::floorHeight, wardrobeLength),
        gravity,
        House::floorColor
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, 0.0f, -House::interiorWallThickness),
    Vec3(wardrobeWallSpacing, House::floorHeight + House::height, House::interiorWallThickness)
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness + wardrobeWallSpacing + 2 * wardrobeDoorWidth, 0.0f, -House::interiorWallThickness),
    Vec3(wardrobeWallSpacing, House::floorHeight + House::height, House::interiorWallThickness)
    ));

    // HEADER
    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness + wardrobeWallSpacing, House::floorHeight + wardrobeHeight, -House::interiorWallThickness),
    Vec3(2 * wardrobeDoorWidth, totalHeight - (House::floorHeight + wardrobeHeight), House::interiorWallThickness)
    ));

    // BELOW HEADER
    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness + wardrobeWallSpacing, 0.0f, -House::interiorWallThickness),
    Vec3(2 * wardrobeDoorWidth, House::floorHeight, House::interiorWallThickness),
        gravity,
        House::floorColor
    ));

    // CEILING
    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, House::floorHeight + wardrobeHeight, 0.0f),
    Vec3(wardrobeWidth, House::floorHeight, wardrobeLength)
    ));

    walls->addChildren(wardrobe);

    // LAVATORY
    // Object* wardrobe = new Object(Vec3<GLfloat>(width - wardrobeWidth, 0.0f, totalLength - wardrobeLength - House::interiorWallThickness));


    return walls;
}

FramedWindow *Bedroom1::addDoor() {
    return nullptr;
}

Object *Bedroom1::addFurniture() {
    return nullptr;
}