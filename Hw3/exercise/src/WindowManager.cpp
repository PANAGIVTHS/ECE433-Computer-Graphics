#include "WindowManager.h"
#include "GameManager.h"
#include "InputManager.h"

// Private methods

void WindowManager::init() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    if (gameMode) {
        currentWindow = glutEnterGameMode();
        width = glutGameModeGet(GLUT_GAME_MODE_WIDTH);
        height = glutGameModeGet(GLUT_GAME_MODE_HEIGHT);
    } else {
        width = initialWidth;
        height = initialHeight;
        glutInitWindowSize(width, height);
        glutInitWindowPosition(10, 10);
        currentWindow = glutCreateWindow("Team 1 - Assignment 3");
    }

    InputManager::init();
    GameManager::onWindowUpdate(width, height, true);
    
    glutIgnoreKeyRepeat(1);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(width/2, height/2);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);
}

void WindowManager::reshape(int newWidth, int newHeight) {
    WindowManager::width = width;
    WindowManager::height = height;
    aspect = (GLdouble) width / (GLdouble) height;
    
    GameManager::onWindowUpdate(newWidth, newHeight, false);
}

// Public methods

void WindowManager::switchMode() {
    if (gameMode)
        glutLeaveGameMode();
    else
        glutDestroyWindow(currentWindow);

    gameMode = !gameMode;
    init();
}

void WindowManager::init(void (*display) (), void (*idle) ()) {
    WindowManager::display = display;
    WindowManager::idle = idle;
    init();
}

GLint WindowManager::getWidth() {
    return width;
}

GLint WindowManager::getHeight() {
    return height;
}

GLdouble WindowManager::getAspect() {
    return aspect;
}

bool WindowManager::isGameMode() {
    return gameMode;
}