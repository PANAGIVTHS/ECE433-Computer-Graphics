#include "Garage.h"
#include "Lantern.h"

void Garage::addAll() {
    addChildren(addFloor());
    addChildren(addRightSide());
    addChildren(addBackSide());
    addChildren(addCeiling());
    Object *frontSide = addFrontSide();
    addChildren(frontSide);
    frontSide->addChildren(addDoor());
}

Object *Garage::addFloor() {
    Object *floor = new AnchoredCube(Vec3(0.0f, 0.0f, ceilingOffset + wallThickness), Vec3(width, House::floorHeight, length - 3 * wallThickness), gravity, House::floorColor, TextureID::NONE, wallMaterial);
    return floor;
}

Object *Garage::addRightSide() {
    GLint ridgeCount = 23;
    Object* rightWall = new RidgedWall(
        Vec3(width, 0.0f, ceilingOffset),
        Vec3(wallThickness, height, length),
        Vec3<GLfloat>(1.0f, 0.0f, 0.0f),
        ridgeCount,
        ridgeThickness,
        false,
        color,
        TextureID::NONE,
        wallMaterial
    );

    return rightWall;
}

Object *Garage::addBackSide() {
    GLint ridgeCount = 9;
    Object* backWall = new RidgedWall(
        Vec3(0.0f, 0.0f, ceilingOffset),
        Vec3(width, height, wallThickness),
        Vec3<GLfloat>(0.0f, 0.0f, -1.0f),
        ridgeCount,
        ridgeThickness,
        false,
        color,
        TextureID::NONE,
        wallMaterial
    );

    return backWall;
}

Object *Garage::addCeiling() {
    GLfloat ceilingWidth = length + 2 * ceilingOffset;
    GLfloat ceilingHeight = width + wallThickness + ceilingOffset;
    Object* ceiling = new AnchoredCube(
        Vec3(0.0f, height, 0.0f),
        Vec3(ceilingHeight, wallThickness, ceilingWidth),
        false,
        color,
        TextureID::NONE,
        wallMaterial
    );

    return ceiling;
}

Object *Garage::addFrontSide() {
    GLint ridgeCount = 1;

    Object* frontWallContainer = new Object(
        Vec3(0.0f, 0.0f, ceilingOffset + length - 2 * wallThickness),
        gravity,
        color,
        TextureID::NONE
    );

    RidgedWall* leftPanel = new RidgedWall(
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(sidePanelWidth, height, wallThickness),
        Vec3<GLfloat>(0.0f, 0.0f, 1.0f),
        ridgeCount,
        ridgeThickness,
        gravity,
        color,
        TextureID::NONE,
        wallMaterial
    );
    frontWallContainer->addChildren(leftPanel);

    RidgedWall* rightPanel = new RidgedWall(
        Vec3(sidePanelWidth + doorWidth, 0.0f, 0.0f),
        Vec3(sidePanelWidth, height, wallThickness),
        Vec3<GLfloat>(0.0f, 0.0f, 1.0f),
        ridgeCount,
        ridgeThickness,
        gravity,
        color,
        TextureID::NONE,
        wallMaterial
    );
    frontWallContainer->addChildren(rightPanel);

    ridgeCount = 7;
    RidgedWall* header = new RidgedWall(
        Vec3(sidePanelWidth, doorHeight, 0.0f),
        Vec3(doorWidth, headerHeight, wallThickness),
        Vec3<GLfloat>(0.0f, 0.0f, 1.0f),
        ridgeCount,
        ridgeThickness,
        gravity,
        color,
        TextureID::NONE,
        wallMaterial
    );
    frontWallContainer->addChildren(header);

    // LATERNS
    Vec3<GLfloat> laternPos = Vec3(0.6f, height * 0.4f, 0.5f);

    Lantern* leftLantern = new Lantern(laternPos);
    leftLantern->setRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f));
    frontWallContainer->addChildren(leftLantern);

    laternPos.x = width - laternPos.x;
    Lantern* rightLantern = new Lantern(laternPos);
    rightLantern->setRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f));
    frontWallContainer->addChildren(rightLantern);

    return frontWallContainer;
}

Object *Garage::addDoor() {
    GLint rows = 4;
    GLint cols = 2;
    GLfloat padding = 0.1f;
    GLfloat glassW = (doorWidth  - (padding * (cols + 1))) / cols;
    GLfloat glassH = (doorHeight - (padding * (rows + 1))) / rows;

    FramedWindow *garageDoor = new FramedWindow(
        Vec3(sidePanelWidth, 0.0f, House::exteriorWallThickness * 0.5f),
        rows,
        cols,
        glassW,
        glassH,
        padding,
        padding,
        1.0f,
        gravity,
        color,
        texture,
        material
    );

    return garageDoor;
}