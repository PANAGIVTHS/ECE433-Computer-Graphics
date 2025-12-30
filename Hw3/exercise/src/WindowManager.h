#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "GameState.h"
#include "UserInput.h"

class WindowManager {
private:
    static constexpr GLint initialWidth = 800, initialHeight = 600;
    inline static GLint width = initialWidth;
    inline static GLint height = initialHeight;
    inline static GLdouble aspect = (GLdouble) width / height;
    inline static int currentWindow = -1;
    inline static bool gameMode = true;
    inline static void (*display)();
    inline static void (*idle) ();

    inline static void reshape(int newWidth, int newHeight) {
        WindowManager::width = width;
        WindowManager::height = height;
        aspect = (GLdouble) width / (GLdouble) height;
        
        GameState::onWindowUpdate(newWidth, newHeight, false);
    }

    inline static void init() {
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

        UserInput::init();
        GameState::onWindowUpdate(width, height, true);
        
        glutIgnoreKeyRepeat(1);
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(width/2, height/2);

        glutDisplayFunc(display);
        glutIdleFunc(idle);
        glutReshapeFunc(reshape);

        glEnable(GL_DEPTH_TEST);
    }
public:
    inline static void switchMode() {
        if (gameMode)
            glutLeaveGameMode();
        else
            glutDestroyWindow(currentWindow);

        gameMode = !gameMode;
        init();
    }

    inline static void init(void (*display) (), void (*idle) ()) {
        WindowManager::display = display;
        WindowManager::idle = idle;
        init();
    }

    inline static GLint getWidth() {
        return width;
    }

    inline static GLint getHeight() {
        return height;
    }

    inline static GLdouble getAspect() {
        return aspect;
    }

    inline static bool isGameMode() {
        return gameMode;
    }
};

#endif
