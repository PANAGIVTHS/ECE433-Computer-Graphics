#include "Object.h"

void Object::draw() {
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);
    glColor3f(1, 0, 1);
    glutSolidCube(1.0f);

    glPopMatrix();
}