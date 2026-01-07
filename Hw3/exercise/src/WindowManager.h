#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class WindowManager {
private:
    static constexpr GLint initialWidth = 800, initialHeight = 600;
    static constexpr GLint initialX = 1930, initialY = 10;
    inline static GLint width = initialWidth;
    inline static GLint height = initialHeight;
    inline static GLdouble aspect = (GLdouble) width / height;
    inline static int currentWindow = -1;
    inline static bool gameMode = false;
    inline static void (*display) () = nullptr;
    inline static void (*idle) () = nullptr;

    static void init();
    static void reshape(int newWidth, int newHeight);
public:
    static void switchMode();
    static void init(void (*display) (), void (*idle) ());

    static GLint getWidth();
    static GLint getHeight();
    static GLdouble getAspect();
    static bool isGameMode();

    //fps related
    static void drawFPS(float fps);
};

#endif
