#include "Bedroom1.h"

#include "Bathroom.h"
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
    return new AnchoredCuboid(
            Vec3<GLfloat>(0.0f, 0.0f, House::ridgeThickness + House::exteriorWallThickness),
            Vec3<GLfloat>(width, floorHeight, length),
            gravity,
            House::floorColor,
            TextureID::NONE,
            MaterialID::MATTE
        );
}

FramedWindow *Bedroom1::addWindows() {
    FramedWindow *window = new FramedWindow(
        Vec3<GLfloat>(backWallSideWidth, windowHeightOffset + floorHeight, House::exteriorWallThickness * 0.5f),
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
    Vec3(House::exteriorWallThickness, House::height + floorHeight, 2 * House::interiorWallThickness + length + lavatoryWidth),
        Vec3(1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    // FRONT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(totalWidth - House::exteriorWallThickness - House::ridgeThickness - DiningRoom::width, 0.0f, totalLength - House::interiorWallThickness),
    Vec3(DiningRoom::width, House::height + floorHeight, House::interiorWallThickness)
    ));

    // BACK WALL
    Object* backWalls = new Object(Vec3<GLfloat>(0.0f,0,House::ridgeThickness));

    ridgeCount = 2;
    backWalls->addChildren(new RidgedWall(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(backWallSideWidth, House::height + floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    ridgeCount = 4;
    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth, 0.0f, 0.0f),
    Vec3(window->getDimensions().x, floorHeight + windowHeightOffset, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth, floorHeight + windowHeightOffset + window->getDimensions().y, 0.0f),
    Vec3(window->getDimensions().x, totalHeight - (floorHeight + windowHeightOffset + window->getDimensions().y), House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    ridgeCount = 7;
    backWalls->addChildren(new RidgedWall(Vec3(backWallSideWidth + window->getDimensions().x, 0.0f, 0.0f),
    Vec3(totalWidth - (House::ridgeThickness + backWallSideWidth + window->getDimensions().x), House::height + floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    walls->addChildren(backWalls);

    // WARDROBE
    Object* wardrobe = new Object(Vec3<GLfloat>(width - wardrobeWidth, 0.0f, totalLength - wardrobeLength - House::interiorWallThickness));

    wardrobe->addChildren(new AnchoredCube(Vec3(0.0f, 0.0f, -House::interiorWallThickness),
    Vec3(House::interiorWallThickness, floorHeight + House::height, wardrobeLength + House::interiorWallThickness)
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, 0.0f, 0.0f),
    Vec3(wardrobeWidth, floorHeight, wardrobeLength),
        gravity,
        House::floorColor
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, 0.0f, -House::interiorWallThickness),
    Vec3(wardrobeWallSpacing, floorHeight + House::height, House::interiorWallThickness)
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness + wardrobeWallSpacing + 2 * wardrobeDoorWidth, 0.0f, -House::interiorWallThickness),
    Vec3(wardrobeWallSpacing, floorHeight + House::height, House::interiorWallThickness)
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness + wardrobeWallSpacing, floorHeight + wardrobeHeight, -House::interiorWallThickness),
    Vec3(2 * wardrobeDoorWidth, totalHeight - (floorHeight + wardrobeHeight), House::interiorWallThickness)
    ));

    wardrobe->addChildren(new AnchoredCube(Vec3(House::interiorWallThickness, floorHeight + wardrobeHeight, 0.0f),
    Vec3(wardrobeWidth, floorHeight, wardrobeLength)
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