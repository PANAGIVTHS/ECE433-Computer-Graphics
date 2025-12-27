#include "Object.h"

std::vector<Object *> ObjectHandler::objects;

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