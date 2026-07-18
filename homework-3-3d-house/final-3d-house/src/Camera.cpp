#include "Camera.h"
#include "GameManager.h"
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

    Vec3 upVector(0.0f, -1.0f, 0.0f);
    Vec3 ccw = direction.cross(upVector).normalize();

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
    GLfloat maxPitch = GameManager::maxPitch;
    pitch += angle;
    if (pitch > maxPitch)
        pitch = maxPitch;
    else if (pitch < -maxPitch)
        pitch = -maxPitch;

    updateDirection();
}  

void Camera::set() {
    gluLookAt(position.x, position.y, position.z, position.x + direction.x, position.y + direction.y, position.z + direction.z, up.x, up.y, up.z);

    LightingManager::getConfig(flashlightID).position = position;
}

void Camera::toggleFlashlight() {
    flashlight = !flashlight;
    LightingManager::toggleLight(flashlightID, flashlight);
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

Vec3<GLfloat> Camera::getDirection() {
    return direction;
}

Vec3<GLfloat>& Camera::getVelocity() {
    return velocity;
}

int Camera::getFlashlightID() {
    return flashlightID;
}

LightConfig Camera::getFlashlightConfig() {
    return flashlightConfig;
}

void Camera::setVelocity(Vec3<GLfloat> velocity) {
    this->velocity = velocity;
}

void Camera::update() {
    if (gravity)
        velocity.y -= GameManager::gravity * GameManager::dt;
    position += velocity * (GLfloat) GameManager::dt;

    if (position.y <= 1.7) { // Temporary ground collision
        position.y = 1.7;
        velocity.y = 0;
    }
}