#ifndef USERINPUT_H
#define USERINPUT_H
#include "Camera.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

class UserInput {
private:
    static bool pressedKeys[6];
    static Camera *camera;
    static GLfloat speed, sensitivity;

    static void keyboardUp(unsigned char key, int x, int y);
    static void keyboardDown(unsigned char key, int x, int y);
    static void mouseMovePassive(int x, int y);
    static void updateMovement();
    static void updateRotation();
public:
    static void init(Camera &camera, GLfloat speed, GLfloat sensitivity) {
        UserInput::camera = &camera;
        UserInput::speed = speed;
        UserInput::sensitivity = sensitivity; 
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
        glutPassiveMotionFunc(mouseMovePassive);
    };

    static void updateCamera();
};

#endif