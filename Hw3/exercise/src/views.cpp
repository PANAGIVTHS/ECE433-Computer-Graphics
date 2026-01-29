#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <algorithm>
#include <iostream>

#include "Object.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "LightingManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

void init(int argc, char *argv[]);
void display();
void setupViewport(int x, int y, int w, int h, 
                   GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, 
                   GLfloat upX, GLfloat upY, GLfloat upZ);
void displayScene(Vec3<GLfloat> camPos);

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'w' - Move forward.\n");
	printf("'a' - Move left.\n");
	printf("'s' - Move backward.\n");
	printf("'d' - Move right.\n");
	printf("'SPACE' - Move up.\n");
	printf("'z' - Move down.\n");
#ifndef MOUSE_ROTATION
	printf("'i' - Look up.\n");
	printf("'j' - Look left.\n");
	printf("'k' - Look down.\n");
	printf("'l' - Look right.\n");
#endif
	printf("'ESC' - Quit the application.\n");
	printf("'F11' - Toggle fullscreen.\n");
#ifdef MOUSE_ROTATION
	printf("\n");
    printf("Move the mouse to look around!\n");
#endif

    glutMainLoop();
    return 0;
}

void init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    WindowManager::init(display, nullptr, 0);
    MaterialManager::init();
    TextureManager::init();
    GameManager::init();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    int w = WindowManager::getWidth();
    int h = WindowManager::getHeight();

    Vec3<GLfloat> camPos = Vec3(0.0f, 0.0f, 3.0f);
    setupViewport(0, 0, w, h, 
                  camPos.x, camPos.y, camPos.z,
                  0.0f, 1.0f, 0.0f);
    displayScene(camPos);

    // setupViewport(w, h, w, h, 
    //               0.0f, 0.0f, 0.0f,
    //               0.0f, 1.0f, 0.0f);
    // displayScene();

    // setupViewport(0, 0, w, h, 
    //               0.0f, 0.0f, 0.0f,
    //               0.0f, 1.0f, 0.0f);
    // displayScene();

    // setupViewport(w, 0, w, h, 
    //               0.0f, 0.0f, 0.0f,
    //               0.0f, 1.0f, 0.0f);
    // displayScene();

    glutSwapBuffers();
}

void setupViewport(int x, int y, int w, int h, 
                   GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, 
                   GLfloat upX, GLfloat upY, GLfloat upZ) {
    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat zoom = 100.0f; 
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    if (w <= h)
        glOrtho(-zoom, zoom, -zoom/aspect, zoom/aspect, 1.0, 100.0);
    else
        glOrtho(-zoom*aspect, zoom*aspect, -zoom, zoom, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,
              0.0, 0.0, 0.0,
              upX, upY, upZ);
}

void displayScene(Vec3<GLfloat> camPos) {
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    
    // //! Sort transparent objects (Farthest -> Nearest)
    std::sort(trans.begin(), trans.end(), [camPos](Object* a, Object* b) {
        return (a->getWorldPosition() - camPos).magSq() > (b->getWorldPosition() - camPos).magSq();
    });

    glDepthMask(GL_FALSE);
    for (Object *o : trans) {
        o->draw();
    }
    glDepthMask(GL_TRUE);
}