#include "Garage.h"

void Garage::addAll() {
    addRightSide();
    addBackSide();
    addCeiling();
    Object *frontSide = addFrontSide();
    addDoor(frontSide);
}

Object *Garage::addRightSide() {
    GLint ridgeCount = 23;
    Object* rightWall = new RidgedWall(
        Vec3(0.0f, height/2.0f, 0.0f),
        Vec3(width, height, wallThickness),
        ridgeCount,
        ridgeThickness,
        false,
        color
    );
    rightWall->setRotation(90, Vec3(0.0f, 1.0f, 0.0f));
    addChildren(rightWall);

    return rightWall;
}

Object *Garage::addBackSide() {
    GLint ridgeCount = 9;
    ridgeThickness = 0.1f;
    Object* backWall = new RidgedWall(
        Vec3(-(length+wallThickness)/2, height/2.0f, -(width - wallThickness)/2),
        Vec3(length, height, wallThickness),
        ridgeCount,
        ridgeThickness,
        false,
        color
    );
    backWall->setRotation(180, Vec3(0.0f, 1.0f, 0.0f));
    addChildren(backWall);

    return backWall;
}

Object *Garage::addCeiling() {
    GLfloat ceilingOffset = 0.6f;
    GLfloat ceilingWidth = width + 2 * ceilingOffset;
    GLfloat ceilingHeight = length + wallThickness + ceilingOffset;
    Object* ceiling = new Cube(
        Vec3(-(length - ceilingOffset)/2.0f, height + wallThickness / 2.0f, 0.0f),
        Vec3(ceilingHeight, wallThickness, ceilingWidth),
        false,
        color
    );
    addChildren(ceiling);

    return ceiling;
}

Object *Garage::addFrontSide() {
    GLint ridgeCount = 1;

    Object* frontWallContainer = new Object(
        Vec3(-length - wallThickness / 2.0f, 0.0f, width/2.0f-wallThickness),
        gravity,
        color,
        TextureID::NONE
    );

    addChildren(frontWallContainer);

    Object* leftPanel = new RidgedWall(
        Vec3(sidePanelWidth / 2.0f, height / 2.0f, 0.0f),
        Vec3(sidePanelWidth, height, wallThickness),
        ridgeCount,
        ridgeThickness,
        gravity,
        color
    );
    frontWallContainer->addChildren(leftPanel);

    Object* rightPanel = new RidgedWall(
        Vec3(sidePanelWidth + doorWidth + sidePanelWidth / 2.0f, height / 2.0f, 0.0f),
        Vec3(sidePanelWidth, height, wallThickness),
        ridgeCount,
        ridgeThickness,
        gravity,
        color
    );
    frontWallContainer->addChildren(rightPanel);

    ridgeCount = 7;
    Object* header = new RidgedWall(
        Vec3(sidePanelWidth + doorWidth / 2.0f, doorHeight + headerHeight / 2.0f, 0.0f),
        Vec3(doorWidth, headerHeight, wallThickness),
        ridgeCount,
        ridgeThickness,
        gravity,
        color
    );
    frontWallContainer->addChildren(header);

    return frontWallContainer;
}

Object *Garage::addDoor(Object *frontSide) {
    GLfloat doorThickness = wallThickness * 0.5f;

    Object* garageDoor = new Object(
        Vec3(sidePanelWidth, 0.0f, 0.0f),
        gravity,
        color,
        TextureID::NONE
    );
    frontSide->addChildren(garageDoor);

    Object* doorFrame = new Cuboid(
        Vec3(doorWidth / 2.0f, doorHeight / 2.0f, 0.0f),
        Vec3(doorWidth, doorHeight, doorThickness),
        gravity,
        color,
        TextureID::NONE
    );
    garageDoor->addChildren(doorFrame);

    Object* glassContainer = new Object(
        Vec3(0.0f, doorHeight, 0.0f),
        gravity,
        color,
        TextureID::NONE
    );
    garageDoor->addChildren(glassContainer);

    // WINDOWS
    int rows = 4;
    int cols = 2;
    GLfloat padding = 0.1f;

    GLfloat glassW = (doorWidth  - (padding * (cols + 1))) / cols;
    GLfloat glassH = (doorHeight - (padding * (rows + 1))) / rows;
    GLfloat glassZ = (doorThickness / 2.0f) + 0.02f;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            GLfloat centerX = padding + (c * (glassW + padding)) + (glassW / 2.0f);
            GLfloat centerY = padding + (r * (glassH + padding)) + (glassH / 2.0f);

            Object* glassPane = new Cuboid(
                Vec3(centerX, -centerY, glassZ),
                Vec3(glassW, glassH, 0.05f),
                false,
                color,
                TextureID::WINDOW
            );

            glassContainer->addChildren(glassPane);
        }
    }

    return garageDoor;
}