#include "Camera.h"
#include <math.h>

void Camera::updateDirection() {
    GLfloat radYaw = toRadians(yaw);
    GLfloat radPitch = toRadians(pitch);

    direction.x = cos(radYaw) * cos(radPitch);
    direction.y = sin(radPitch);
    direction.z = sin(radYaw) * cos(radPitch);
}

void Camera::offset(GLfloat x, GLfloat y, GLfloat z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void Camera::moveTo(GLfloat x, GLfloat y, GLfloat z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Camera::move(Direction dir, GLfloat amount) {
    Vec3 horizontal = direction;
    horizontal.y = 0;
    horizontal.normalize();

    Vec3 ccw = horizontal;
    ccw.y = -ccw.x;
    ccw.x = ccw.z;
    ccw.z = ccw.y;
    ccw.y = 0;

    switch (dir) {
        case FRONT:
            position += horizontal * amount;
            break;
        case BACK:
            position -= horizontal * amount;
            break;
        case LEFT:
            position += ccw * amount;
            break;
        case RIGHT:
            position -= ccw * amount;
            break;
        case UP:
            position.y += amount;
            break;
        case DOWN:
            position.y -= amount;
            break;
    }
}

void Camera::rotateYaw(GLfloat angle) {
    yaw += angle;
    updateDirection();
}

void Camera::rotatePitch(GLfloat angle) {
    pitch += angle;
    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    updateDirection();
}  

void Camera::set() {
    gluLookAt(position.x, position.y, position.z, position.x + direction.x, position.y + direction.y, position.z + direction.z, up.x, up.y, up.z);
}

bool Camera::hasGravity() {
    return gravity;
}

void Camera::setGravity(bool gravity) {
    this->gravity = gravity;
}

Vec3<GLfloat> Camera::getPosition() {
    return position;
}

Vec3<GLfloat>& Camera::getVelocity() {
    return velocity;
}

void Camera::setVelocity(Vec3<GLfloat>& velocity) {
    this->velocity = velocity;
}

void Camera::applyVelocity() {
    this->position += this->velocity;
}