#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "Object.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "LightingManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <algorithm>
#include <vector>
#include "Spline.h"

void init(int argc, char *argv[]);
void display();
void idle();

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'w' - Move forward.\n");
	printf("'a' - Move left.\n");
	printf("'s' - Move backward.\n");
	printf("'d' - Move right.\n");
	printf("'SPACE' - Jump/Move up.\n");
	printf("'z' - Move down.\n");
#ifndef MOUSE_ROTATION
	printf("'i' - Look up.\n");
	printf("'j' - Look left.\n");
	printf("'k' - Look down.\n");
	printf("'l' - Look right.\n");
#endif
	printf("'f' - Toggle flight mode.\n");
	printf("'e' - Toggle camera flashlight.\n");
	printf("'q/ESC' - Quit the application.\n");
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
    const std::string rootPath = (argc == 2) ? argv[1] : "./";
    GameManager::setRootPath(rootPath);
    WindowManager::init(display, idle);
    LightingManager::init();
    MaterialManager::init();
    TextureManager::init();
    GameManager::init();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GameManager::getCamera()->set();
    LightingManager::updateAllLights();
    
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    Vec3<float> camPos = GameManager::getCamera()->getPosition();
    
    //! Sort transparent objects (Farthest -> Nearest)
    std::sort(trans.begin(), trans.end(), [camPos](Object* a, Object* b) {
        return (a->getWorldPosition() - camPos).magSq() > (b->getWorldPosition() - camPos).magSq();
    });

    glDepthMask(GL_FALSE);
    for (Object *o : trans) {
        o->draw();
    }
    glDepthMask(GL_TRUE);

    //fps related
    WindowManager::drawFPS(GameManager::getFPS());

    glutSwapBuffers();
}

void idle() {
    #if defined(MODEL)
    GameManager::reloadAssets();
    #endif
    GameManager::runGameLoop();
    glutPostRedisplay();
}