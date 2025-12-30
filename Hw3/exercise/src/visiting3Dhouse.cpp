#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "Object.h"
#include "GameManager.h"
#include "WindowManager.h"

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
    WindowManager::init(display, idle);
    GameManager::init();
    GameManager::getEnvironment()->spawn();
    GameManager::getHouse()->spawn();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GameManager::getCamera()->set();
    for (Object *o : ObjectHandler::getObjects())
        o->draw();

    glutSwapBuffers();
}

void idle() {
    GameManager::runGameLoop();
    glutPostRedisplay();
}

