#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "Camera.h"
#include "UserInput.h"
#include "Environment.h"
#include "House.h"

const GLfloat speed = 0.2f;
const GLfloat sensitivity = 0.04f;
const GLdouble fov = 90.0f;
const GLint initialWidth = 800, initialHeight = 600;

GLint width = initialWidth;
GLint height = initialHeight;
GLdouble aspect = (GLdouble) width / height;
bool gameMode = true;
int currentWindow;
Camera *camera;
Environment *environment;
House *house;

void init(int argc, char *argv[]);
void setupWindow(bool gameMode);
void display();
void idle();
void reshape(int newWidth, int newHeight);
void onSpecialKey(int key, int x, int y);
void cleanUp();

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'w' - Move forward.\n");
	printf("'a' - Move left.\n");
	printf("'s' - Move backward.\n");
	printf("'d' - Move right.\n");
	printf("'SPACE' - Move up.\n");
	printf("'z' - Move down.\n");
	printf("'ESC' - Quit the application.\n");
	printf("'F11' - Toggle fullscreen.\n");
	printf("\n");
    printf("Move the mouse to look around!\n");

    glutMainLoop();
    return 0;
}

void init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    camera = new Camera(1.0f, 0.0f, 0.0f);
    environment = new Environment({.red = 0.3828125f, .green = 0.75390625f, .blue = 0.89453125f});
    new Terrain(0, 0, 0);
    new Sphere(-2.5f, 2.5f, -4.0f);
    new Cube(1.0f, 2.5f, -6.0f);
    new Cube(4.5f, 2.5f, -5.0f);
    setupWindow(gameMode);
    environment->apply();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera->set();
    for (Object *o : ObjectHandler::getObjects()) {
        o->draw();
    }

    glutSwapBuffers();
}

void idle() {
    UserInput::updateMovement();
    glutPostRedisplay();
}

void reshape(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    aspect = (GLfloat) width / height;
    UserInput::setDimensions(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

void setupWindow(bool gameMode) {
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

    glutIgnoreKeyRepeat(1);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(width/2, height/2);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutSpecialFunc(onSpecialKey);
    UserInput::init(*camera, speed, sensitivity, width, height, cleanUp);

    glEnable(GL_DEPTH_TEST);
    reshape(width, height);
}

void onSpecialKey(int key, int x, int y) {
    if (key != GLUT_KEY_F11)
        return;
    
    if (gameMode)
        glutLeaveGameMode();
    else
        glutDestroyWindow(currentWindow);

    gameMode = !gameMode;
    setupWindow(gameMode);
}

void cleanUp() {
    delete camera;
    delete environment;
    delete house;
    ObjectHandler::clear();
}