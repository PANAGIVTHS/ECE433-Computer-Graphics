#include "Garage.h"
#include "House.h"
#include "Lantern.h"

void Garage::addAll() {
    addFloor();
    addRightSide();
    addBackSide();
    addCeiling();
    Object *frontSide = addFrontSide();
    addDoor(frontSide);
}

Object *Garage::addFloor() {
    Object *floor = new Cube(-(length + wallThickness)/2, 0, -wallThickness/4, length, floorHeight, width - (2.5f * wallThickness), gravity, concreteColor);
    addChildren(floor);
    return floor;
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

    // LATERNS
    float lanternHeight = height * 0.4f;
    float lanternX = 0.6f;
    float lanternZ = 0.5f;

    Object* leftLantern = new Lantern(lanternX, lanternHeight, lanternZ);
    leftLantern->setRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f));
    frontWallContainer->addChildren(leftLantern);

    Object* rightLantern = new Lantern(length - lanternX, lanternHeight, lanternZ);
    rightLantern->setRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f));
    frontWallContainer->addChildren(rightLantern);

    return frontWallContainer;
}

Object *Garage::addDoor(Object *frontSide) {
    GLfloat doorThickness = wallThickness * 0.5f;
    GLint rows = 4;
    GLint cols = 2;
    GLfloat padding = 0.1f;
    GLfloat glassW = (doorWidth  - (padding * (cols + 1))) / cols;
    GLfloat glassH = (doorHeight - (padding * (rows + 1))) / rows;
    GLfloat frameZ = 0.0f;

    Object* garageDoor = new Object(
        Vec3(sidePanelWidth, 0.0f, 0.0f),
        gravity,
        color,
        TextureID::NONE
    );
    frontSide->addChildren(garageDoor);

    // ---------------------------------------------------------
    // FRAME
    // ---------------------------------------------------------

    // Vertical
    for (int i = 0; i <= cols; i++) {
        GLfloat centerX = (padding / 2.0f) + (i * (glassW + padding));
        GLfloat centerY = doorHeight / 2.0f;

        Object* vBar = new Cube(
            Vec3(centerX, centerY, frameZ),
            Vec3(padding, doorHeight, doorThickness),
            gravity,
            color,
            TextureID::NONE
        );
        garageDoor->addChildren(vBar);
    }

    // Horizontal
    for (int r = 0; r <= rows; r++) {
        for (int c = 0; c < cols; c++) {
            GLfloat centerX = padding + (c * (glassW + padding)) + (glassW / 2.0f);
            GLfloat centerY = (padding / 2.0f) + (r * (glassH + padding));

            Object* hBar = new Cube(
                Vec3(centerX, centerY, frameZ),
                Vec3(glassW, padding, doorThickness),
                gravity,
                color,
                TextureID::NONE
            );
            garageDoor->addChildren(hBar);
        }
    }

    // ---------------------------------------------------------
    // WINDOWS
    // ---------------------------------------------------------
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            GLfloat centerX = padding + (c * (glassW + padding)) + (glassW / 2.0f);
            GLfloat centerY = padding + (r * (glassH + padding)) + (glassH / 2.0f);

            Object* glassPane = new Cuboid(
                Vec3(centerX, centerY, 0.0f),
                Vec3(glassW, glassH, doorThickness * 0.5f),
                gravity,
                color,
                TextureID::WINDOW,
                DEFAULT_MATERIAL,
                TextureConfig(),
                10
            );
            glassPane->optimize();

            garageDoor->addChildren(glassPane);
        }
    }

    return garageDoor;
}