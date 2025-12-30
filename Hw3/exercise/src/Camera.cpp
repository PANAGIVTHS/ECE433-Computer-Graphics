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
    horizontal.normalize();

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
    gluLookAt(pos.x, pos.y, pos.z, pos.x + direction.x, pos.y + direction.y, pos.z + direction.z, up.x, up.y, up.z);
}