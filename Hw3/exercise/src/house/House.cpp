#include "House.h"

#include "DiningRoom.h"
#include "FamilyRoom.h"
#include "Garage.h"
#include "Porch.h"

void House::setup() {
    Porch *porch = new Porch(Vec3<GLfloat>(0, 0, 0), House::scale);
    addChildren(porch);

    FamilyRoom *familyRoom = new FamilyRoom(Vec3<GLfloat>(Porch::secondLayerSpacing + Porch::frontLength + Porch::ceilingSpacing - House::ridgeThickness, 0, Porch::ceilingSpacing + Porch::secondLayerSpacing + Porch::leftLength - House::exteriorWallThickness), House::scale);
    Vec3<GLfloat> familyRoomPosition = familyRoom->getPosition() + Vec3<GLfloat>(0, 0, -FamilyRoom::length);
    familyRoom->setPosition(familyRoomPosition);
    addChildren(familyRoom);

    DiningRoom *diningRoom = new DiningRoom(familyRoom->getPosition() + Vec3<GLfloat>(FamilyRoom::totalWidth, 0, 0), House::scale);
    Vec3<GLfloat> diningRoomPosition = diningRoom->getPosition() + Vec3<GLfloat>(0, 0, -(DiningRoom::totalLength - FamilyRoom::totalLength));
    diningRoom->setPosition(diningRoomPosition);
    addChildren(diningRoom);

    Garage *garage = new Garage(diningRoom->getPosition() + Vec3<GLfloat>(DiningRoom::totalWidth - House::ridgeThickness, 0, - FamilyRoom::doorWidth * 0.5), scale);
    addChildren(garage);

    new BorderCubes(porch);
    new BorderCubes(diningRoom);
    new BorderCubes(familyRoom);
    new BorderCubes(garage);
}
