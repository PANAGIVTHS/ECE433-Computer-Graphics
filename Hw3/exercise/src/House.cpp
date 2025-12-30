#include "House.h"
#include "Object.h"

House::House() {

}

void House::init() {
    
}

void House::spawn() {
    new Sphere(-2.5f, 2.5f, -4.0f);
    new Cube(1.0f, 2.5f, -6.0f);
    new Cube(4.5f, 2.5f, -5.0f);
}