#include "Object.h"
#include "GameManager.h"
#include <algorithm>
#include "TextureManager.h"

// Object implementation

Object::Object(Vec3<GLfloat> position, bool gravity, TextureID texture) : position(position), gravity(gravity), texture(texture) {
    ObjectHandler::addObject(this);
}

Object::~Object() {
    ObjectHandler::removeObject(this);

    for (Object *o : children)
        delete o;
    children.clear();
}

void Object::draw() {
    if (hidden)
        return;

    glPushMatrix();

    glTranslated(position.x, position.y, position.z);

    if (texture != TextureID::NONE) {
        TextureManager::bind(texture);
    }

    drawInternal();

    if (texture != TextureID::NONE) {
        TextureManager::bind(TextureID::NONE);
    }
    for (Object *o : children)
        o->draw();

    glPopMatrix();
}

void Object::update() {
    if (gravity)
        velocity.y -= GameManager::gravity * GameManager::dt;
    this->position += this->velocity * (GLfloat) GameManager::dt;
}

Object *Object::addChildren(Object *o) {
    ObjectHandler::removeObject(o);
    o->setGravity(false);
    o->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
    children.push_back(o);

    return this;
}

bool Object::isHidden() {
    return hidden;
}

void Object::setHidden(bool hidden) {
    this->hidden = hidden;
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

void Object::setPosition(Vec3<GLfloat> position) {
    this->position = position;
}

Vec3<GLfloat>& Object::getVelocity() {
    return velocity;
}

void Object::setVelocity(Vec3<GLfloat> velocity) {
    this->velocity = velocity;
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