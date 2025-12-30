#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class InputManager {
private:
    inline static bool pressedKeys[6] = { false };
#ifndef MOUSE_ROTATION
    inline static bool pressedRotationKeys[4] = { false };
#endif

    static void keyboardUp(unsigned char key, int x, int y);
    static void keyboardDown(unsigned char key, int x, int y);
    static void specialKeyboardDown(int key, int x, int y);
    static void mouseMovePassive(int x, int y);
#ifndef MOUSE_ROTATION
    static void updateCameraRotation();
#endif
public:
    static void init();
    static void applyInputToCamera();
};

#endif