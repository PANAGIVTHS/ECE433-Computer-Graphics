#include "Compound.h"
#include "Object.h"

Compound::Compound() {

}

void Compound::init() {
    
}

void Compound::spawn() {
    new Sphere(-2.5f, 2.5f, -4.0f);
    new Cube(1.0f, 2.5f, -6.0f);
    new Cube(4.5f, 2.5f, -5.0f);
    new Cube(0.0f, 0.0f, 0.0f);
    new Cube(1.0f, 0.0f, 1.0f);
}