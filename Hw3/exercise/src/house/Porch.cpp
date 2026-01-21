#include "Porch.h"

void Porch::addAll() {
    Object *container = new Object(0.0f, 0.0f, 0.0f);
    addChildren(container);

    container->addChildren(addFloor());
    container->addChildren(addCeiling());
    container->addChildren(addPillars());
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