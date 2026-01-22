#include "Bathroom.h"

void Bathroom::addAll() {
    addChildren(addFloor());
    FramedWindow *window = addWindows();
    addChildren(window);
    FramedWindow *door = addDoor();
    addChildren(door);
    addChildren(addWalls(window, door));
    addChildren(addFurniture());
}

Object *Bathroom::addFloor() {
    return new AnchoredCuboid(
            Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness, 0.0f, 0.0f),
            Vec3<GLfloat>(width, floorHeight, length),
            gravity,
            House::floorColor,
            TextureID::NONE,
            MaterialID::MATTE
        );
}

FramedWindow *Bathroom::addWindows() {
    FramedWindow *window = new FramedWindow(
        Vec3<GLfloat>(House::ridgeThickness + House::exteriorWallThickness * 0.5f, windowHeightOffset, leftWallSideWidth),
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
    window->setRotation(-90, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
    window->setPosition(window->getPosition() + Vec3<GLfloat>(window->getDimensions().z, 0.0f, 0.0f));

    return window;
}

Object *Bathroom::addWalls(FramedWindow *window, FramedWindow *door) {
    Object* walls = new Object(Vec3<GLfloat>(0,0,0));

    // RIGHT WINDOW WALL
    GLint ridgeCount = 7;
    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, 0.0f),
    Vec3(House::exteriorWallThickness, House::height + floorHeight, leftWallSideWidth),
        Vec3(-1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    ridgeCount = 4;
    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, leftWallSideWidth),
    Vec3(House::exteriorWallThickness, windowHeightOffset, window->getDimensions().x),
        Vec3(-1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );
    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, windowHeightOffset + window->getDimensions().y, leftWallSideWidth),
    Vec3(House::exteriorWallThickness, totalHeight - windowHeightOffset - window->getDimensions().y, window->getDimensions().x),
        Vec3(-1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );


    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, leftWallSideWidth + window->getDimensions().x),
    Vec3(House::exteriorWallThickness, House::height + floorHeight, totalLength - leftWallSideWidth - window->getDimensions().x),
        Vec3(-1.0f, 0.0f, 0.0f),
        ridgeCount,
        House::ridgeThickness)
    );

    // FRONT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(House::exteriorWallThickness + House::ridgeThickness, 0.0f, length),
    Vec3(width + rightWallInset + House::interiorWallThickness, House::height + floorHeight, House::interiorWallThickness)
    ));

    // RIGHT WALL
    walls->addChildren(new AnchoredCube(
        Vec3(House::exteriorWallThickness + House::ridgeThickness + width , 0.0f, 0.0f),
    Vec3(House::interiorWallThickness, House::height + floorHeight, rightWallSideWidth)
    ));

    walls->addChildren(new AnchoredCube(
        Vec3(House::exteriorWallThickness + House::ridgeThickness + width, 0.0f, rightWallSideWidth),
    Vec3(House::interiorWallThickness, floorHeight, doorWidth)
    ));

    walls->addChildren(new AnchoredCube(
        Vec3(House::exteriorWallThickness + House::ridgeThickness + width, floorHeight + doorHeight, rightWallSideWidth),
    Vec3(House::interiorWallThickness, totalHeight - floorHeight - doorHeight, doorWidth)
    ));

    walls->addChildren(new AnchoredCube(
        Vec3(House::exteriorWallThickness + House::ridgeThickness + width, 0.0f, rightWallSideWidth + doorWidth),
    Vec3(House::interiorWallThickness, House::height + floorHeight, length - rightWallSideWidth - doorWidth)
    ));

    return walls;
}

FramedWindow *Bathroom::addDoor() {
    return nullptr;
}

Object *Bathroom::addFurniture() {
    return nullptr;
}