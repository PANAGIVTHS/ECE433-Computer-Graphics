#include "DiningRoom.h"

void DiningRoom::addAll() {
    addChildren(addFloor());
    FramedWindow *window = addWindows();
    addChildren(window);
    FramedWindow *door = addDoor();
    addChildren(door);
    addChildren(addWalls(window, door));
    addChildren(addFurniture());
}

Object *DiningRoom::addFloor() {
    return new AnchoredCuboid(
            Vec3<GLfloat>(0.0f, 0.0f, 0.0f),
            Vec3<GLfloat>(width, House::floorHeight, length),
            gravity,
            House::floorColor,
            TextureID::NONE,
            MaterialID::MATTE
        );
}

FramedWindow *DiningRoom::addWindows() {
    FramedWindow *window = new FramedWindow(
        Vec3<GLfloat>(frontWallSideWidth, House::floorHeight + windowHeightOffset, length),
        windowRows,
        windowColumns,
        windowWidth,
        windowHeight,
        doorFrame,
        windowSpacing,
        scale,
        gravity,
        House::darkColor,
        texture,
        material
    );

    return window;
}

Object *DiningRoom::addWalls(FramedWindow *window, FramedWindow *door) {
    Object* walls = new Object(Vec3<GLfloat>(0,0,0));

    // FRONT WALLS
    walls->addChildren(new RidgedWall(Vec3(0.0f, 0.0f, length),
    Vec3(frontWallSideWidth, House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(frontWallSideWidth, 0.0f, length),
    Vec3(window->getDimensions().x, windowHeightOffset + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(frontWallSideWidth, House::floorHeight + windowHeightOffset + window->getDimensions().y, length),
    Vec3(window->getDimensions().x, totalHeight - (House::floorHeight + windowHeightOffset + window->getDimensions().y), House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(frontWallSideWidth + window->getDimensions().x, 0.0f, length),
    Vec3(width - window->getDimensions().x - frontWallSideWidth, House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    // RIGHT WALLS
    walls->addChildren(new RidgedWall(Vec3(width, 0.0f, length + House::exteriorWallThickness - rightWallSideWidth - House::exteriorWallThickness),
    Vec3(House::exteriorWallThickness, House::height + House::floorHeight, rightWallSideWidth + House::exteriorWallThickness),
        Vec3(1.0f, 0.0f, 0.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(width, door->getDimensions().y + House::floorHeight, length - rightWallSideWidth - door->getDimensions().x),
    Vec3(House::exteriorWallThickness, totalHeight - (door->getDimensions().y + House::floorHeight), door->getDimensions().x),
        Vec3(1.0f, 0.0f, 0.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(width, 0.0f, 0.0f),
    Vec3(House::exteriorWallThickness, House::height + House::floorHeight, length - rightWallSideWidth - door->getDimensions().x),
        Vec3(1.0f, 0.0f, 0.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new AnchoredCube(
    Vec3(width, 0.0f, length - rightWallSideWidth - door->getDimensions().x),
    Vec3(House::exteriorWallThickness, House::floorHeight, door->getDimensions().x),
        gravity,
        House::floorColor,
        texture,
        material
    ));

    return walls;
}

FramedWindow *DiningRoom::addDoor() {
    FramedWindow *door = new FramedWindow(
        Vec3<GLfloat>(width, House::floorHeight, length - rightWallSideWidth),
        6,
        2,
        windowWidth,
        windowHeight,
        doorFrame,
        windowSpacing,
        scale,
        gravity,
        House::darkColor,
        texture,
        material
    );
    door->setPosition(door->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -door->getDimensions().x));
    return door;
}

Object *DiningRoom::addFurniture() {
    return nullptr;
}
