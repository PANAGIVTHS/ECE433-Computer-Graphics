#ifndef USERINPUT_H
#define USERINPUT_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

class UserInput {
private:
    static bool pressedKeys[6];
#ifndef MOUSE_ROTATION
    static bool pressedRotationKeys[4];
#endif

    static void keyboardUp(unsigned char key, int x, int y);
    static void keyboardDown(unsigned char key, int x, int y);
    static void specialKeyboardDown(int key, int x, int y);
    static void mouseMovePassive(int x, int y);
#ifndef MOUSE_ROTATION
    static void updateRotation();
#endif
public:
    static void init() {
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialFunc(specialKeyboardDown);
#ifdef MOUSE_ROTATION
        glutPassiveMotionFunc(mouseMovePassive);
#endif
    };

    static void updateMovement();
};

#endif