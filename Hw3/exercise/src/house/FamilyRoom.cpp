#include "FamilyRoom.h"

void FamilyRoom::addAll() {
    addChildren(addFloor());
    Object *windows = addWindows();
    addChildren(windows);
    addChildren(addDoor(dynamic_cast<FramedWindow *>(windows->getChildren()[0])));
    addChildren(addWalls(dynamic_cast<FramedWindow *>(windows->getChildren()[0])));
    addChildren(addFurniture());
}

Object *FamilyRoom::addFloor() {
    return new AnchoredCuboid(
            Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness, 0.0f, 0.0f),
            Vec3<GLfloat>(width, House::floorHeight, length),
            gravity,
            House::floorColor,
            TextureID::NONE,
            MaterialID::MATTE
        );
}

Object *FamilyRoom::addWalls(FramedWindow *window) {
    Object* walls = new Object(Vec3<GLfloat>(0,0,0));

    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, 0.0f),
    Vec3(House::exteriorWallThickness, House::height + House::floorHeight, length),
        Vec3(-1.0f, 0.0f, 0.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness, 0.0f, length),
    Vec3(House::exteriorWallThickness + frontWallSideWidth, House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness + House::exteriorWallThickness + frontWallSideWidth + (windowCount + 1) * window->getDimensions().x, 0.0f, length),
    Vec3(width - frontWallSideWidth - (windowCount + 1) * window->getDimensions().x, House::height + House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new RidgedWall(Vec3(House::ridgeThickness + House::exteriorWallThickness + frontWallSideWidth, House::floorHeight + window->getDimensions().y, length),
    Vec3((windowCount + 1) * window->getDimensions().x, totalHeight - window->getDimensions().y - House::floorHeight, House::exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing,
        House::ridgeThickness)
    );

    walls->addChildren(new AnchoredCuboid(
            Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness + frontWallSideWidth, 0.0f, length),
            Vec3<GLfloat>(window->getDimensions().x * 4, House::floorHeight, House::exteriorWallThickness),
            gravity,
            House::floorColor
    ));

    return walls;
}

Object *FamilyRoom::addWindows() {
    Object* windows = new Object(Vec3<GLfloat>(House::exteriorWallThickness + House::ridgeThickness + frontWallSideWidth,House::floorHeight,length));

    FramedWindow *first = new FramedWindow(
        Vec3<GLfloat>(0.0f, 0.0f, House::exteriorWallThickness * 0.5f),
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
    windows->addChildren(first);

    for (int i = 1; i < windowCount; i++) {
        GLfloat xOff = i * first->getDimensions().x;

        FramedWindow *window = new FramedWindow(
            Vec3<GLfloat>(xOff, 0.0f, House::exteriorWallThickness * 0.5f),
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
        windows->addChildren(window);
    }

    return windows;
}

Object *FamilyRoom::addDoor(FramedWindow *window) {
    GLfloat doorLeftX = House::ridgeThickness + House::exteriorWallThickness + frontWallSideWidth + windowCount * window->getDimensions().x;

    FramedWindow *door = new FramedWindow(
        Vec3<GLfloat>(0.0f, 0.0f, 0.0f),
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

    GLfloat doorTotalWidth = door->getDimensions().x;
    Object* hingePivot = new Object(
        Vec3<GLfloat>(doorLeftX + doorTotalWidth, House::floorHeight, length + House::exteriorWallThickness)
    );
    door->setPosition(Vec3<GLfloat>(-doorTotalWidth, 0.0f, 0.0f));
    hingePivot->addChildren(door);

    GLfloat handleWidth = 0.2f;
    GLfloat handleHeight = 0.4f;
    GLfloat handleLength = 0.01f;
    AnchoredCube *firstLayer = new AnchoredCube(
        Vec3(doorFrame + windowSpacing, door->getDimensions().y * 0.5f - handleHeight * 0.5f, door->getDimensions().z),
        Vec3(handleWidth, handleHeight, handleLength),
        gravity,
        House::darkColor,
        texture,
        material
    );
    door->addChildren(firstLayer);

    AnchoredCube *secondLayer = new AnchoredCube(
        Vec3(doorFrame + windowSpacing + handleWidth * 0.1f, door->getDimensions().y * 0.5f - handleHeight * 0.4f, door->getDimensions().z + handleLength),
        Vec3(handleWidth * 0.8f, handleHeight * 0.8f, handleLength),
        gravity,
        House::darkColor,
        texture,
        material
    );
    door->addChildren(secondLayer);

    hingePivot->setRotation(-90, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));

    return hingePivot;
}

Object *FamilyRoom::addFurniture() {
    return nullptr;
}
