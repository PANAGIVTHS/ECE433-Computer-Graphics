#include "InputManager.h"
#include "GameManager.h"
#include "WindowManager.h"

void InputManager::init() {
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialFunc(specialKeyboardDown);
#ifdef MOUSE_ROTATION
        glutPassiveMotionFunc(mouseMovePassive);
#endif
}

void InputManager::keyboardUp(unsigned char key, int x, int y) {
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

void InputManager::keyboardDown(unsigned char key, int x, int y) {
    Camera *camera = GameManager::getCamera();
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
            if (camera->hasGravity()) 
                camera->getVelocity().y += GameManager::jumpSpeed;
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
        case 'f': 
        case 'F': 
            camera->setGravity(!camera->hasGravity());
            camera->getVelocity().y = 0;
            break;
        case 27: // Escape
            GameManager::cleanUp();
            exit(0);
        default:
            break;
    }
}

void InputManager::specialKeyboardDown(int key, int x, int y) {
    if (key != GLUT_KEY_F11)
        return;
    
    WindowManager::switchMode();
}

#ifdef MOUSE_ROTATION
void InputManager::mouseMovePassive(int x, int y) {
    GLint width = WindowManager::getWidth(), height = WindowManager::getHeight();

    GLint centerX = width / 2;
    GLint centerY = height / 2;

    if (x == centerX && y == centerY)
        return;

    pending_yaw += (x - centerX);
    pending_pitch += (centerY - y);

    glutWarpPointer(centerX, centerY);
}
#endif

void InputManager::applyInputToCamera() {
    Camera *camera = GameManager::getCamera();
    GLfloat distance = GameManager::speed * GameManager::dt;

    if (pressedKeys[FRONT])
        camera->move(FRONT, distance);
    if (pressedKeys[BACK])
        camera->move(BACK, distance);
    if (pressedKeys[LEFT])
        camera->move(LEFT, distance);
    if (pressedKeys[RIGHT])
        camera->move(RIGHT, distance);
    if (pressedKeys[UP] && !camera->hasGravity())
        camera->move(UP, distance);
    if (pressedKeys[DOWN] && !camera->hasGravity())
        camera->move(DOWN, distance);
    
#ifndef MOUSE_ROTATION
    updateCameraRotation();
#else
    camera->rotatePitch(pending_pitch * GameManager::sensitivity);
    camera->rotateYaw(pending_yaw * GameManager::sensitivity);
    pending_pitch = 0.0f;
    pending_yaw = 0.0f;
#endif
}

#ifndef MOUSE_ROTATION
void InputManager::updateCameraRotation() {
    Camera *camera = GameManager::getCamera();
    GLfloat angle = GameManager::sensitivity * GameManager::dt * kbSensitivityMultiplier;
    GLfloat yaw = 0;
    GLfloat pitch = 0;

    if (pressedRotationKeys[FRONT])
        pitch += angle;
    if (pressedRotationKeys[BACK])
        pitch -= angle;
    if (pressedRotationKeys[LEFT])
        yaw += angle;
    if (pressedRotationKeys[RIGHT])
        yaw -= angle;

    camera->rotateYaw(yaw);
    camera->rotatePitch(pitch);
}
#endif