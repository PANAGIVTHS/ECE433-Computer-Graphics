#include "House.h"

void House::setup() {
    Garage *garage = new Garage(0, 0, 0);
    garage->setMaterial(MaterialID::MATTE, true);
    addChildren(garage);
}