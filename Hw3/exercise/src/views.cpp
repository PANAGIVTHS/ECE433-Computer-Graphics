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
void setupPerspective(int x, int y, int w, int h, 
                      Vec3<GLfloat> eye, Vec3<GLfloat> center, Vec3<GLfloat> up);
void setupOrtho(int x, int y, int w, int h, GLfloat zoom, bool topDown);
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

    int W = WindowManager::getWidth();
    int H = WindowManager::getHeight();
    int halfW = W / 2;
    int halfH = H / 2;
    House *house = GameManager::getHouse();

    // ---------------------------------------------------------
    // Botton left floor plan
    // ---------------------------------------------------------
    house->getRoof()->setHidden(true); 
    // house->getGarageCeiling()->setHidden(true);

    setupOrtho(0, 0, halfW, halfH, 20.0f, true);
    displayScene(Vec3(10.0f, 50.0f, 10.0f));

    // ---------------------------------------------------------
    // Bottom right floor plan with roof
    // ---------------------------------------------------------
    house->getRoof()->setHidden(false); 
    // house->getGarageCeiling()->setHidden(false);

    setupOrtho(halfW, 0, halfW, halfH, 20.0f, true);
    displayScene(Vec3(10.0f, 50.0f, 10.0f));

    // ---------------------------------------------------------
    // Top left front view perspective
    // ---------------------------------------------------------
    // displayScene(camPos);

    // ---------------------------------------------------------
    // Top right interior cycling
    // ---------------------------------------------------------
    // displayScene(camPos);

    glutSwapBuffers();
}

void setupPerspective(int x, int y, int w, int h, 
                      Vec3<GLfloat> eye, Vec3<GLfloat> center, Vec3<GLfloat> up) {
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, 
              center.x, center.y, center.z, 
              up.x, up.y, up.z);
}

void setupOrtho(int x, int y, int w, int h, GLfloat zoom, bool topDown) {
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    if (w <= h)
        glOrtho(-zoom, zoom, -zoom/aspect, zoom/aspect, -100.0, 100.0);
    else
        glOrtho(-zoom*aspect, zoom*aspect, -zoom, zoom, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (topDown) {
        gluLookAt(10.0, 50.0, 10.0,
                  10.0, 0.0, 10.0,
                  0.0, 0.0, -1.0);
    }
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