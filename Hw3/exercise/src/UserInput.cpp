#include "UserInput.h"

Camera *UserInput::camera = nullptr;
float UserInput::speed; 
float UserInput::sensitivity;
bool UserInput::pressedKeys[6] = {false};
int UserInput::width, UserInput::height;
void (*UserInput::cleanUp)() = nullptr;
#ifndef MOUSE_ROTATION
    bool UserInput::pressedRotationKeys[4] = {false};
#endif

void UserInput::keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            pressedKeys[FRONT] = false;
            break; 
        case 'a':
        case 'A':
            pressedKeys[LEFT] = false;
            break;    
        case 's':
        case 'S':
            pressedKeys[BACK] = false;
            break;
        case 'd':
        case 'D':
            pressedKeys[RIGHT] = false;
            break;
        case 'z':
        case 'Z':
            pressedKeys[DOWN] = false;
            break;
#ifndef MOUSE_ROTATION
        case 'i':
        case 'I':
            pressedRotationKeys[FRONT] = false;
            break;
        case 'l':
        case 'L':
            pressedRotationKeys[LEFT] = false;
            break;
        case 'k':
        case 'K':
            pressedRotationKeys[BACK] = false;
            break;
        case 'j':
        case 'J':
            pressedRotationKeys[RIGHT] = false;
            break;
#endif
        case 32: // Space
            pressedKeys[UP] = false;
            break;
        default:
            break;
    }
}

void UserInput::keyboardDown(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            pressedKeys[FRONT] = true;
            break; 
        case 'a':
        case 'A':
            pressedKeys[LEFT] = true;
            break;    
        case 's':
        case 'S':
            pressedKeys[BACK] = true;
            break;
        case 'd':
        case 'D':
            pressedKeys[RIGHT] = true;
            break;
        case 'z':
        case 'Z':
            pressedKeys[DOWN] = true;
            break;
        case 32: // Space
            pressedKeys[UP] = true;
            break;
#ifndef MOUSE_ROTATION
        case 'i':
        case 'I':
            pressedRotationKeys[FRONT] = true;
            break;
        case 'l':
        case 'L':
            pressedRotationKeys[LEFT] = true;
            break;
        case 'k':
        case 'K':
            pressedRotationKeys[BACK] = true;
            break;
        case 'j':
        case 'J':
            pressedRotationKeys[RIGHT] = true;
            break;
#endif
        case 27: // Escape
            if (cleanUp) cleanUp();
            exit(0);
        default:
            break;
    }
}

void UserInput::mouseMovePassive(int x, int y) {
    GLint centerX = width / 2;
    GLint centerY = height / 2;

    if (x == centerX && y == centerY)
        return;

    GLfloat yaw = (x - centerX) * sensitivity;
    GLfloat pitch = (centerY - y) * sensitivity;
    camera->rotateYaw(yaw);
    camera->rotatePitch(pitch);

    glutWarpPointer(centerX, centerY);
}

void UserInput::updateMovement() {
    if (pressedKeys[FRONT])
        camera->move(FRONT, speed);
    if (pressedKeys[BACK])
        camera->move(BACK, speed);
    if (pressedKeys[LEFT])
        camera->move(LEFT, speed);
    if (pressedKeys[RIGHT])
        camera->move(RIGHT, speed);
    if (pressedKeys[UP])
        camera->move(UP, speed);
    if (pressedKeys[DOWN])
        camera->move(DOWN, speed);
}

#ifndef MOUSE_ROTATION
void UserInput::updateRotation() {
    GLfloat yaw = 0;
    GLfloat pitch = 0;
    const int sensAmplification = 50;

    if (pressedRotationKeys[FRONT])
        pitch += sensitivity * sensAmplification;
    if (pressedRotationKeys[BACK])
        pitch -= sensitivity * sensAmplification;
    if (pressedRotationKeys[LEFT])
        yaw += sensitivity * sensAmplification;
    if (pressedRotationKeys[RIGHT])
        yaw -= sensitivity * sensAmplification;

    camera->rotateYaw(yaw);
    camera->rotatePitch(pitch);
}
#endif