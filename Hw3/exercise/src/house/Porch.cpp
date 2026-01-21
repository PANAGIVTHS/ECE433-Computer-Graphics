#include "Porch.h"

#include "FamilyRoom.h"
#include "../AssetLoader.h"

void Porch::addAll() {
    Object *container = new Object(0.0f, 0.0f, 0.0f);
    addChildren(container);

    container->addChildren(addFloor());
    container->addChildren(addCeiling());
    container->addChildren(addPillars());

    FlowerPot *pot1 = new FlowerPot(Vec3(
        ceilingSpacing + frontLength + secondLayerSpacing + House::exteriorWallThickness + House::ridgeThickness + FamilyRoom::frontWallSideWidth,
        firstLayerHeight + secondLayerHeight,
        ceilingSpacing + secondLayerSpacing + leftLength + House::exteriorWallThickness + House::ridgeThickness)
    );
    FlowerPot *pot2 = new FlowerPot(Vec3(
        ceilingSpacing + frontLength + secondLayerSpacing + House::exteriorWallThickness + House::ridgeThickness + FamilyRoom::frontWallSideWidth + 2 * FamilyRoom::doorWidth,
        firstLayerHeight + secondLayerHeight,
        ceilingSpacing + secondLayerSpacing + leftLength + House::exteriorWallThickness + House::ridgeThickness)
    );
    container->addChildren(pot1);
    container->addChildren(pot2);
}

Object *Porch::addFloor() {
    Object *floorContainer = new Object(ceilingSpacing, 0, ceilingSpacing);

    // --- Bottom ---
    Object* l1_F = new AnchoredCube(
        Vec3(secondLayerSpacing, 0.0f, leftLength + secondLayerSpacing),
        Vec3(frontWidth, firstLayerHeight, frontLength),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l1_F);

    Object* l1_L = new AnchoredCube(
        Vec3(secondLayerSpacing, 0.0f, secondLayerSpacing),
        Vec3(frontLength, firstLayerHeight, leftLength),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l1_L);

    // --- Top Floor ---

    Object* l2_F = new AnchoredCuboid(
        Vec3(0.0f, firstLayerHeight, leftLength + secondLayerSpacing),
        Vec3(frontWidth + 2 * secondLayerSpacing, secondLayerHeight, frontLength + secondLayerSpacing),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l2_F);

    Object* l2_L = new AnchoredCuboid(
        Vec3(0.0f, firstLayerHeight, 0.0f),
        Vec3(frontLength + secondLayerSpacing, secondLayerHeight, leftLength + secondLayerSpacing),
        gravity, floorColor, texture, floorMaterial
    );
    floorContainer->addChildren(l2_L);

    return floorContainer;
}

Object *Porch::addCeiling() {
    Object *ceilingContainer = new Object(0, firstLayerHeight + secondLayerHeight + pillarHeight, 0);

    Object* ceilA = new AnchoredCuboid(
        Vec3(0.0f, 0.0f, leftLength + secondLayerSpacing + ceilingSpacing),
        Vec3(frontWidth + 2 * (secondLayerSpacing + ceilingSpacing), ceilingHeight, frontLength + secondLayerSpacing + ceilingSpacing),
        false, ceilingColor, TextureID::NONE, ceilingMaterial
    );
    ceilingContainer->addChildren(ceilA);

    Object* ceilB = new AnchoredCuboid(
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(frontLength + secondLayerSpacing + ceilingSpacing, ceilingHeight, leftLength + secondLayerSpacing + ceilingSpacing),
        false, ceilingColor, TextureID::NONE, ceilingMaterial
    );
    ceilingContainer->addChildren(ceilB);

    return ceilingContainer;
}

Object *Porch::addPillars() {
    Object *pillarContainer = new Object(ceilingSpacing, firstLayerHeight + secondLayerHeight, ceilingSpacing);

    TextureID pTex = texture;
    MaterialID pMat = floorMaterial;

    Object* p1 = new AnchoredCuboid(
        Vec3( pillarInset, 0.0f,  pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p1);

    Object* p2 = new AnchoredCuboid(
        Vec3(pillarInset, 0.0f, leftLength + frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p2);

    Object* p3 = new AnchoredCuboid(
        Vec3(frontLength, 0.0f, leftLength + frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p3);

    Object* p4 = new AnchoredCuboid(
        Vec3(frontWidth - pillarInset, 0.0f, leftLength + frontLength - pillarInset),
        Vec3(pillarWidth, pillarHeight, pillarWidth),
        gravity, floorColor, pTex, pMat
    );
    pillarContainer->addChildren(p4);

    return pillarContainer;
}

void FlowerPot::addAll() {
    Object* pot = new AnchoredCuboid(
        Vec3<GLfloat>(0.0f, 0.0f, 0.0f),
        Vec3<GLfloat>(potSize, potHeight, potSize),
        DEFAULT_GRAVITY,
        clayColor,
        TextureID::NONE,
        MaterialID::MATTE
    );
    this->addChildren(pot);

    Object* soil = new AnchoredCuboid(
        Vec3<GLfloat>(potSize * 0.1, potHeight - (0.02f * scale), potSize * 0.1),
        Vec3<GLfloat>(potSize * 0.8f, 0.05f * scale, potSize * 0.8f),
        DEFAULT_GRAVITY,
        soilColor,
        TextureID::NONE,
        MaterialID::MATTE
    );
    this->addChildren(soil);

    Rose* rose = new Rose(Vec3<GLfloat>(potSize * 0.5f, potHeight - 0.1f, potSize * 0.3f));
    rose->setScale(Vec3<GLfloat>(scale, scale, scale));
    this->addChildren(rose);
}