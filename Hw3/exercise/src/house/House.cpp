#include "House.h"
#include "Garage.h"
#include "Porch.h"

void House::setup() {
    Porch *porch = new Porch(Vec3<GLfloat>(0, 0, 0));
    addChildren(porch);

    BorderCubes *cubes = new BorderCubes(porch);
    addChildren(cubes);

    Vec3<GLfloat> garagePos = porch->getPosition() + Vec3<GLfloat>(Porch::totalWidth, 0, Porch::totalLength - Garage::totalLength);
    Garage *garage = new Garage(garagePos);
    addChildren(garage);

    BorderCubes *cubes1 = new BorderCubes(garage);
    addChildren(cubes1);
}
