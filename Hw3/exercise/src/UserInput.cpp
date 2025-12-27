#include "UserInput.h"

Camera *UserInput::camera = nullptr;
float UserInput::speed; 
float UserInput::sensitivity;
bool UserInput::pressedKeys[6] = {false};

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
        case 27: // Escape
            exit(0);
        default:
            break;
    }
}

void UserInput::mouseMovePassive(int x, int y) {

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

void UserInput::updateRotation() {
}

void UserInput::updateCamera() {
    updateMovement();
    updateRotation();
}