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
    static GLint width, height;
    static void (*cleanUp) ();
#ifndef MOUSE_ROTATION
    static bool pressedRotationKeys[4];
#endif

    static void keyboardUp(unsigned char key, int x, int y);
    static void keyboardDown(unsigned char key, int x, int y);
    static void mouseMovePassive(int x, int y);
#ifndef MOUSE_ROTATION
    static void updateRotation();
#endif
public:
    static void init(Camera &camera, GLfloat speed, GLfloat sensitivity, GLint width, GLint height, void (*cleanUp) ()) {
        UserInput::camera = &camera;
        UserInput::speed = speed;
        UserInput::sensitivity = sensitivity; 
        UserInput::width = width;
        UserInput::height = height;
        UserInput::cleanUp = cleanUp;
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
#ifdef MOUSE_ROTATION
        glutPassiveMotionFunc(mouseMovePassive);
#endif
    };

    static void setDimensions(GLint width, GLint height) {
        UserInput::width = width;
        UserInput::height = height;
    }

    static void updateMovement();
};

#endif