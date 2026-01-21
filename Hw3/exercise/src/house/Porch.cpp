#include "Porch.h"

void Porch::addAll() {
    addFloor();
    addCeiling();
    addPillars();
}

void Porch::addFloor() {
    GLfloat fW = frontWidth;
    GLfloat fL = frontLength;
    GLfloat lL = leftLength;
    GLfloat s = secondLayerSpacing;

    Object *floorContainer = new Object(0, 0, 0);
    addChildren(floorContainer);

    // --- Bottom ---
    GLfloat l1_Y = firstLayerHeight / 2.0f;
    GLfloat l1_H = firstLayerHeight;

    Object* l1_A = new Cuboid(
        Vec3(fW / 2.0f, l1_Y, fL / 2.0f),
        Vec3(fW, l1_H, fL),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l1_A);

    Object* l1_B = new Cuboid(
        Vec3(fL / 2.0f, l1_Y, -lL / 2.0f),
        Vec3(fL, l1_H, lL),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l1_B);

    // --- Top Floor ---
    GLfloat l2_Y = firstLayerHeight + secondLayerHeight / 2.0f;
    GLfloat l2_H = secondLayerHeight;

    Object* l2_A = new Cuboid(
        Vec3(fW / 2.0f, l2_Y, fL / 2.0f),
        Vec3(fW + 2 * s, l2_H, fL + 2 * s),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l2_A);

    Object* l2_B = new Cuboid(
        Vec3(fL / 2.0f, l2_Y, - s - lL / 2.0f),
        Vec3(fL + 2 * s, l2_H, lL),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l2_B);
}

void Porch::addCeiling() {
    GLfloat fW = frontWidth;
    GLfloat fL = frontLength;
    GLfloat lL = leftLength;
    GLfloat sW = ceilingWidthSpacing;
    GLfloat sL = ceilingLengthSpacing;

    GLfloat yPos = firstLayerHeight + secondLayerHeight + pillarHeight + ceilingHeight / 2.0f;

    Object *ceilingContainer = new Object(0, 0, 0);
    addChildren(ceilingContainer);

    Object* ceilA = new Cuboid(
        Vec3(fW / 2.0f, yPos, (fL + sL) / 2.0f),
        Vec3(fW + 2 * (sW + secondLayerSpacing), ceilingHeight, fL + 2 * secondLayerSpacing + sL),
        false, ceilingColor, TextureID::NONE, ceilingMaterial
    );
    ceilingContainer->addChildren(ceilA);

    Object* ceilB = new Cuboid(
        Vec3((fL - sW) / 2.0f, yPos, -secondLayerSpacing - (lL + sL) / 2.0f),
        Vec3(fL + 2 * secondLayerSpacing + sW, ceilingHeight, lL + sL),
        false, ceilingColor, TextureID::NONE, ceilingMaterial
    );
    ceilingContainer->addChildren(ceilB);
}

void Porch::addPillars() {
    // Pillars sit on the top of the second floor layer
    GLfloat yPos = firstLayerHeight + secondLayerHeight + pillarHeight / 2.0f;

    Object *pillarContainer = new Object(0, 0, 0);
    addChildren(pillarContainer);

    TextureID pTex = texture;
    MaterialID pMat = floorMaterial;

    Object* p1 = new Cuboid(
        Vec3(frontWidth - pillarInset, yPos, frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p1);

    Object* p2 = new Cuboid(
        Vec3(pillarInset, yPos, frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p2);

    Object* p3 = new Cuboid(
        Vec3(pillarInset, yPos, -leftLength + pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p3);

    Object* p4 = new Cuboid(
        Vec3(frontLength + pillarWidth, yPos, frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p4);
}