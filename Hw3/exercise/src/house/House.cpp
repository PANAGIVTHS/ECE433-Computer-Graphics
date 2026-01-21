#include "House.h"
#include "Garage.h"
#include "Porch.h"

void House::setup() {
    Porch *porch = new Porch(0, 0, 0);
    addChildren(porch);

    Vec3<GLfloat> garagePos = porch->getPosition() + Vec3<GLfloat>(Porch::frontWidth + Garage::length, 0, 0);
    Garage *garage = new Garage(garagePos);
    addChildren(garage);
}
