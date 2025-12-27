#include "Object.h"

void Object::draw() {
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);
    drawInternal();

    glPopMatrix();
}