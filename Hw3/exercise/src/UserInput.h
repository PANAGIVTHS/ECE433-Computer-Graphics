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
    static int width, height;

    static void keyboardUp(unsigned char key, int x, int y);
    static void keyboardDown(unsigned char key, int x, int y);
    static void mouseMovePassive(int x, int y);
public:
    static void init(Camera &camera, GLfloat speed, GLfloat sensitivity, int width, int height) {
        UserInput::camera = &camera;
        UserInput::speed = speed;
        UserInput::sensitivity = sensitivity; 
        UserInput::width = width;
        UserInput::height = height;
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
        glutPassiveMotionFunc(mouseMovePassive);
    };

    static void updateMovement();
};

#endif