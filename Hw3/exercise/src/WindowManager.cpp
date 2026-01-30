#include "WindowManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include <string>


//fps related
#ifdef __linux__
#include <GL/glx.h>
#endif

//fps related
void WindowManager::drawFPS(float fps) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2i(10, 20);

    std::string fpsStr = "FPS: " + std::to_string((int)fps);
    for (char c : fpsStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

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
        glutInitWindowPosition(initialX, initialY);
        currentWindow = glutCreateWindow("Team 1 - Assignment 3");
    }
    
    //fps related
    #if defined(__linux__) && defined(VSYNC)
        Display* dpy = glXGetCurrentDisplay();
        GLXDrawable drawable = glXGetCurrentDrawable();

        typedef void (*PFNGLXSWAPINTERVALEXTPROC)(Display*, GLXDrawable, int);
        PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = nullptr;

        glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");

        if (glXSwapIntervalEXT) {
            glXSwapIntervalEXT(dpy, drawable, 0);
        } else {
            typedef int (*PFNGLXSWAPINTERVALSGIPROC)(int);
            PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");
            if (glXSwapIntervalSGI) {
                glXSwapIntervalSGI(0);
            }
        }
    #endif

    if (mode) {
        glutIgnoreKeyRepeat(1);
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(width/2, height/2);
    }

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    
    if (mode)
        InputManager::init();
    GameManager::onWindowUpdate(width, height, true);
}

void WindowManager::reshape(int newWidth, int newHeight) {
    WindowManager::width = newWidth;
    WindowManager::height = newHeight;
    aspect = (GLdouble) newWidth / (GLdouble) newHeight;
    
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

void WindowManager::init(void (*display) (), void (*idle) (), int mode) {
    WindowManager::mode = mode;
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

int WindowManager::getMode() {
    return mode;
}