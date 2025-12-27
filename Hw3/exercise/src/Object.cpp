#include "Object.h"

// Object implementation

Object::Object(Vec3<GLfloat> &pos): pos(pos) {
    ObjectHandler::addObject(this);
}

Object::Object(GLfloat x, GLfloat y, GLfloat z): pos(Vec3(x, y, z)) { 
    ObjectHandler::addObject(this); 
}

void Object::draw() {
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);
    drawInternal();

    glPopMatrix();
}

// ObjectHandler implementation

std::vector<Object *> ObjectHandler::objects;

void ObjectHandler::addObject(Object *o) {
    ObjectHandler::objects.push_back(o);
}

std::vector<Object *> &ObjectHandler::getObjects() {
    return objects;
}

void ObjectHandler::clear() {
    for (Object *o : objects) {
        delete o;
    }
    objects.clear();
}

// Object subclasses implementation

// Terrain
void Terrain::drawInternal() {
    glScaled(10, 0.1, 10);
    glTranslated(0, -10, 0);
    glutSolidCube(10.0f);
}

// Cube
void Cube::drawInternal() {
    glutSolidCube(1.0f);
}

// Sphere
void Sphere::drawInternal() {
    glutSolidSphere(1.0f, 20, 20);
}