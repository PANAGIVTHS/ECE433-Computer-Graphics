#include "Bedroom2.h"

#include "Bathroom.h"

void Bedroom2::addAll() {
    addChildren(addFloor());
    FramedWindow *window = addWindows();
    addChildren(window);
    FramedWindow *door = addDoor();
    addChildren(door);
    addChildren(addWalls(window, door));
    addChildren(addFurniture());
}

Object *Bedroom2::addFloor() {
    return new AnchoredCuboid(
            Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness, 0.0f, House::ridgeThickness + House::exteriorWallThickness),
            Vec3<GLfloat>(width, floorHeight, length),
            gravity,
            House::floorColor,
            TextureID::NONE,
            MaterialID::MATTE
        );
}

FramedWindow *Bedroom2::addWindows() {
    FramedWindow *window = new FramedWindow(
        Vec3<GLfloat>(House::ridgeThickness + backWallSideWidth, windowHeightOffset + floorHeight, House::exteriorWallThickness * 0.5f),
        windowRows,
        windowColumns,
        windowWidth,
        windowHeight,
        windowFrame,
        windowSpacing,
        scale,
        gravity,
        House::floorColor,
        texture,
        material
    );

    return window;
}

Object *Bedroom2::addWalls(FramedWindow *window, FramedWindow *door) {
    Object* walls = new Object(Vec3<GLfloat>(0,0,0));

    // RIGHT WALL
    GLint ridgeCount = 7;
    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, House::ridgeThickness + House::exteriorWallThickness),
    Vec3(House::exteriorWallThickness, House::height + floorHeight, House::interiorWallThickness + length),
        Vec3(-1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    // FRONT WALL
    Object* frontWalls = new Object(Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness,0,House::ridgeThickness + House::exteriorWallThickness + length));

    frontWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, 0.0f),
    Vec3(Bathroom::width + House::interiorWallThickness + frontWallInset, House::height + floorHeight, House::interiorWallThickness)
    ));

    frontWalls->addChildren(new AnchoredCube(
        Vec3(Bathroom::width + House::interiorWallThickness + frontWallInset, 0.0f, 0.0f),
    Vec3(doorWidth, floorHeight, House::interiorWallThickness)
    ));

    frontWalls->addChildren(new AnchoredCube(
        Vec3(Bathroom::width + House::interiorWallThickness + frontWallInset, floorHeight + doorHeight, 0.0f),
    Vec3(doorWidth, totalHeight - floorHeight - doorHeight, House::interiorWallThickness)
    ));

    frontWalls->addChildren(new AnchoredCube(
        Vec3(Bathroom::width + House::interiorWallThickness + frontWallInset + doorWidth, 0.0f, 0.0f),
    Vec3(
        (House::ridgeThickness + House::exteriorWallThickness + width + House::interiorWallThickness) - (Bathroom::width + House::interiorWallThickness + frontWallInset + doorWidth + House::exteriorWallThickness + House::ridgeThickness),
        House::height + floorHeight,
        House::interiorWallThickness)
    ));

    walls->addChildren(frontWalls);

    // RIGHT WALL
    Object* rightWalls = new Object(Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness + width,0,House::ridgeThickness + House::exteriorWallThickness));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f , 0.0f, 0.0f),
    Vec3(House::interiorWallThickness, House::height + floorHeight, rightWallSideWidth)
    ));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, rightWallSideWidth),
    Vec3(House::interiorWallThickness, floorHeight, wardrobeWidth)
    ));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, floorHeight + wardrobeHeight, rightWallSideWidth),
    Vec3(House::interiorWallThickness, totalHeight - floorHeight - wardrobeHeight, wardrobeWidth)
    ));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, rightWallSideWidth + wardrobeWidth),
    Vec3(House::interiorWallThickness, floorHeight, wardrobeWidth)
    ));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, floorHeight + wardrobeHeight, rightWallSideWidth + wardrobeWidth),
    Vec3(House::interiorWallThickness, totalHeight - floorHeight - wardrobeHeight, wardrobeWidth)
    ));

    rightWalls->addChildren(new AnchoredCube(
        Vec3(0.0f, 0.0f, rightWallSideWidth + 2 * wardrobeWidth),
    Vec3(House::interiorWallThickness, House::height + floorHeight, totalLength - rightWallSideWidth - 2 * wardrobeWidth - House::exteriorWallThickness - House::ridgeThickness - House::interiorWallThickness)
    ));

    walls->addChildren(rightWalls);

    // BACK WALL
    Object* backWalls = new Object(Vec3<GLfloat>(House::ridgeThickness,0,House::ridgeThickness));

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
    walls->addChildren(new RidgedWall(Vec3(backWallSideWidth + window->getDimensions().x, 0.0f, 0.0f),
    Vec3(totalWidth - (House::ridgeThickness + backWallSideWidth + window->getDimensions().x), House::height + floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(backWallSideWidth + window->getDimensions().x, 0.0f, 0.0f),
    Vec3(totalWidth - (House::ridgeThickness + backWallSideWidth + window->getDimensions().x), House::height + floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(backWallSideWidth + window->getDimensions().x, 0.0f, 0.0f),
    Vec3(totalWidth - (House::ridgeThickness + backWallSideWidth + window->getDimensions().x), House::height + floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    return walls;
}

FramedWindow *Bedroom2::addDoor() {
    return nullptr;
}

Object *Bedroom2::addFurniture() {
    return nullptr;
}