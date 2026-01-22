#include "House.h"

#include "Bathroom.h"
#include "Bedroom2.h"
#include "DiningRoom.h"
#include "FamilyRoom.h"
#include "Garage.h"
#include "Porch.h"

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

    Garage *garage = new Garage(diningRoom->getPosition() + Vec3<GLfloat>(DiningRoom::totalWidth - House::ridgeThickness, 0, - FamilyRoom::doorWidth * 0.5), scale);
    addChildren(garage);
    garage->optimize();

    Bathroom *bathroom = new Bathroom(familyRoom->getPosition(), House::scale);
    bathroom->setPosition(bathroom->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bathroom::totalLength));
    addChildren(bathroom);
    bathroom->optimize();

    Bedroom2 *bedroom2 = new Bedroom2(bathroom->getPosition(), House::scale);
    bedroom2->setPosition(bedroom2->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bedroom2::totalLength));
    addChildren(bedroom2);
    bedroom2->optimize();

    // new BorderCubes(porch);
    // new BorderCubes(diningRoom);
    // new BorderCubes(familyRoom);
    // new BorderCubes(garage);
    // new BorderCubes(bedroom2);
}
