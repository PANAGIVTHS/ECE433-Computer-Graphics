#include "Object.h"
#include <algorithm>

// Object implementation

Object::Object(Vec3<GLfloat> &position): position(position) {
    ObjectHandler::addObject(this);
}

Object::Object(GLfloat x, GLfloat y, GLfloat z): position(Vec3(x, y, z)) { 
    ObjectHandler::addObject(this); 
}

Object::Object(Vec3<GLfloat> &position, bool gravity): position(position), gravity(gravity) {
    ObjectHandler::addObject(this);
}

Object::Object(GLfloat x, GLfloat y, GLfloat z, bool gravity): position(Vec3(x, y, z)), gravity(gravity) { 
    ObjectHandler::addObject(this); 
}

Object::~Object() {
    ObjectHandler::removeObject(this);
}

void Object::draw() {
    glPushMatrix();

    glTranslated(position.x, position.y, position.z);
    drawInternal();

    glPopMatrix();
}

bool Object::hasGravity() {
    return gravity;
}

void Object::setGravity(bool gravity) {
    this->gravity = gravity;
}

Vec3<GLfloat>& Object::getPosition() {
    return position;
}

void Object::setPosition(Vec3<GLfloat>& position) {
    this->position = position;
}

Vec3<GLfloat>& Object::getVelocity() {
    return velocity;
}

void Object::setVelocity(Vec3<GLfloat>& velocity) {
    this->velocity = velocity;
}

void Object::update() {
    this->position += this->velocity;
}

// ObjectHandler implementation

std::vector<Object *> ObjectHandler::objects;

void ObjectHandler::addObject(Object *o) {
    ObjectHandler::objects.push_back(o);
}

void ObjectHandler::removeObject(Object *o) {
    std::vector<Object *>::iterator it = std::find(objects.begin(), objects.end(), o);
    if (it != objects.end()) {
        std::iter_swap(it, objects.end() - 1);
        objects.pop_back();
    }
}

std::vector<Object *> &ObjectHandler::getObjects() {
    return objects;
}

void ObjectHandler::clear() {
    while(!objects.empty()) {
        delete objects.back();
    }
}

// Object subclasses implementation

// Terrain
void Terrain::drawInternal() {
    glScaled(10, 0.1, 10);
    glTranslated(0, -10, 0);
    glColor3f(0, 1.0f, 0.2f);
    glutSolidCube(10.0f);
}

// Cube
void Cube::drawInternal() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.0f);
}

// Sphere
void Sphere::drawInternal() {
    glColor3f(1.0f, 0.0f, 1.0f);
    glutSolidSphere(1.0f, 20, 20);
}