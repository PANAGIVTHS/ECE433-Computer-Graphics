#include "Camera.h"

void Camera::offset(GLfloat x, GLfloat y, GLfloat z) {
    pos.x += x;
    pos.y += y;
    pos.z += z;
}

void Camera::moveTo(GLfloat x, GLfloat y, GLfloat z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void Camera::move(Direction dir, GLfloat amount) {
    Vec3 horizontal = direction;
    horizontal.y = 0;
    Vec3 ccw = horizontal;
    ccw.y = -ccw.x;
    ccw.x = ccw.z;
    ccw.z = ccw.y;
    ccw.y = 0;

    switch (dir) {
        case FRONT:
            pos += horizontal * amount;
            break;
        case BACK:
            pos -= horizontal * amount;
            break;
        case LEFT:
            pos += ccw * amount;
            break;
        case RIGHT:
            pos -= ccw * amount;
            break;
        case UP:
            pos.y += amount;
            break;
        case DOWN:
            pos.y -= amount;
            break;
    }
}

void Camera::moveCenter(GLfloat x, GLfloat y, GLfloat z) {
    direction.x = x;
    direction.y = y;
    direction.z = z;
}

void Camera::offsetCenter(GLfloat x, GLfloat y, GLfloat z) {
    direction.x += x;
    direction.y += y;
    direction.z += z;
}

void Camera::set() {
    gluLookAt(pos.x, pos.y, pos.z, pos.x + direction.x, pos.y + direction.y, pos.z + direction.z, up.x, up.y, up.z);
}