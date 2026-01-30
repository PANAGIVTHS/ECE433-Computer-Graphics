#include "House.h"

#include "Bathroom.h"
#include "Bedroom1.h"
#include "Bedroom2.h"
#include "DiningRoom.h"
#include "FamilyRoom.h"
#include "Garage.h"
#include "Hallway.h"
#include "Porch.h"
#include "../Model.h"
#include "../AssetLoader.h"

void House::setup() {
    Porch *porch = new Porch(Vec3<GLfloat>(0, 0, 0), House::scale);
    addChildren(porch);
    porch->optimize();

    FamilyRoom *familyRoom = new FamilyRoom(Vec3<GLfloat>(Porch::secondLayerSpacing + Porch::frontLength + Porch::ceilingSpacing - House::ridgeThickness, 0, Porch::ceilingSpacing + Porch::secondLayerSpacing + Porch::leftLength - House::exteriorWallThickness), House::scale);
    Vec3<GLfloat> familyRoomPosition = familyRoom->getPosition() + Vec3<GLfloat>(0, 0, -FamilyRoom::length);
    familyRoom->setPosition(familyRoomPosition);
    addChildren(familyRoom);
    familyRoom->optimize();

    DiningRoom *diningRoom = new DiningRoom(familyRoom->getPosition() + Vec3<GLfloat>(FamilyRoom::totalWidth, 0, 0), House::scale);
    Vec3<GLfloat> diningRoomPosition = diningRoom->getPosition() + Vec3<GLfloat>(0, 0, -(DiningRoom::totalLength - FamilyRoom::totalLength));
    diningRoom->setPosition(diningRoomPosition);
    addChildren(diningRoom);
    diningRoom->optimize();

    Garage *garage = new Garage(diningRoom->getPosition() + Vec3<GLfloat>(DiningRoom::totalWidth - House::ridgeThickness, 0, -House::garageInset), scale);
    Model *car = new Model("../assets/Car.obj", Vec3(Garage::totalWidth / 2.5f, floorHeight + 0.2f, Garage::totalLength / 2.0f), false, {1.0f, 1.0f, 1.0f}, TextureID::CAR, MaterialID::MATTE);
    car->setRotation(180, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
    car->optimize();
    garage->addChildren(car);
    garageCeiling = garage->getCeiling();
    addChildren(garage);

    Bathroom *bathroom = new Bathroom(familyRoom->getPosition(), House::scale);
    bathroom->setPosition(bathroom->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bathroom::totalLength));
    addChildren(bathroom);
    bathroom->optimize();

    Bedroom2 *bedroom2 = new Bedroom2(bathroom->getPosition(), House::scale);
    bedroom2->setPosition(bedroom2->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bedroom2::totalLength));
    addChildren(bedroom2);
    bedroom2->optimize();

    Bedroom1 *bedroom1 = new Bedroom1(bedroom2->getPosition() + Vec3<GLfloat>(Bedroom2::totalWidth, 0.0f, 0.0f), House::scale);
    addChildren(bedroom1);
    bedroom1->optimize();

    Hallway *hallway = new Hallway(diningRoomPosition, scale);
    Vec3<GLfloat> hallwayPosition = hallway->getPosition() + Vec3<GLfloat>(-Hallway::totalWidth, 0, -House::interiorWallThickness);
    hallway->setPosition(hallwayPosition);
    addChildren(hallway);

    Model *tv = new Model("../assets/tv.obj", Vec3(8.2f, 0.8f, -5.5f), false, {1.0f, 1.0f, 1.0f}, TextureID::TV, MaterialID::MATTE);
    tv->setRotation(180, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
    tv->setScale(0.17, 0.17, 0.17);
    addChildren(tv);
    tv->optimize();

    // PATHWAY
    GLfloat pathWidth = Garage::doorWidth;
    GLfloat pathLength = 16;
    GLfloat pathHeight = 0.02f;

    GLfloat doorOffsetX = Garage::sidePanelWidth;
    GLfloat garageFrontZ = Garage::ceilingOffset + Garage::length - 2 * House::exteriorWallThickness;
    Vec3<GLfloat> pathPos = garage->getPosition() + Vec3(
        doorOffsetX,
        0.0f,
        garageFrontZ
    );

    AnchoredCuboid *pathway = new AnchoredCuboid(pathPos, Vec3(pathWidth, pathHeight, pathLength), false);
    pathway->setTexture(TextureID::STONE, false);
    pathway->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT));
    pathway->setSubdivisions(10);
    addChildren(pathway);
    pathway->optimize();

    GLfloat branchLength = Garage::doorWidth * 2.5;
    GLfloat branchWidth = Garage::doorWidth;
    GLfloat branchZOffset = DiningRoom::totalLength - House::exteriorWallThickness - House::ridgeThickness - (Garage::length - House::garageInset) + Porch::frontLength;
    AnchoredCuboid *pathway1 = new AnchoredCuboid(pathPos + Vec3<GLfloat>(-branchLength, 0.0f, branchZOffset), Vec3(branchLength, pathHeight, branchWidth), false);
    pathway1->setTexture(TextureID::STONE, false);
    pathway1->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT));
    pathway1->setSubdivisions(10);
    addChildren(pathway1);
    pathway1->optimize();

    // ROOF
    roof = new Object(Vec3<GLfloat>(bedroom2->getPosition().x, 0.0f, bedroom2->getPosition().z), House::scale);

    GLfloat ceilingWidth = FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * (exteriorWallThickness + ridgeThickness);
    GLfloat ceilingHeight = 0.2f;
    GLfloat ceilingLength = DiningRoom::totalLength + Bedroom1::totalLength - 2 * (exteriorWallThickness + ridgeThickness);

    roof->addChildren(new AnchoredCube(Vec3((exteriorWallThickness + ridgeThickness), House::height + House::floorHeight, (exteriorWallThickness + ridgeThickness)),
    Vec3(ceilingWidth, ceilingHeight,  ceilingLength),
        gravity,
        House::darkColor
    ));

    roof->addChildren(new TriangularRidgedWall(Vec3(ridgeThickness, House::height + House::floorHeight, Bedroom1::totalLength + DiningRoom::totalLength - exteriorWallThickness - ridgeThickness),
        Vec3(FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * ridgeThickness, roofHeight, exteriorWallThickness),
        Vec3(0.0f, 0.0f, 1.0f),
        House::ridgeSpacing + 0.032,
        House::ridgeThickness,
        gravity,
        House::lightColor
    ));

    roof->addChildren(new TriangularRidgedWall(Vec3(ridgeThickness, House::height + House::floorHeight, ridgeThickness),
        Vec3(FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * ridgeThickness, roofHeight, exteriorWallThickness),
        Vec3(0.0f, 0.0f, -1.0f),
        House::ridgeSpacing + 0.032,
        House::ridgeThickness,
        gravity,
        House::lightColor
    ));

    roof->addChildren(AssetLoader::addAsset("../assets/roof.txt", Vec3<GLfloat>(-bedroom2->getPosition().x, 0.0f, -bedroom2->getPosition().z)));

    addChildren(roof);

}

Object *House::getGarageCeiling() {
    return garageCeiling;
}

Object *House::getRoof() {
    return roof;
}
