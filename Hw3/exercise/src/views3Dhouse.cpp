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

const GLfloat speed = 0.2f;
const GLfloat sensitivity = 0.04f;
const GLdouble fov = 90.0f;
int width = 800;
int height = 600;
GLdouble aspect = (GLdouble) width / height;
bool gameMode = true;
int currentWindow;

Object *obj1 = new Object(-2.5f, 2.5f, -2.0f);
Object *obj2 = new Object(1.0f, 2.5f, -4.0f);
Object *obj3 = new Object(4.5f, 2.5f, -3.0f);
Camera *camera = new Camera(0.0f, 0.0f, 2.0f);
UserInput *userInput;

void init(int argc, char *argv[]);
void setupWindow(bool gameMode);
void display();
void idle();
void reshape(int newWidth, int newHeight);
void onSpecialKey(int key, int x, int y);

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'w' - Move forward.\n");
	printf("'a' - Move left.\n");
	printf("'s' - Move backward.\n");
	printf("'d' - Move right.\n");
	printf("'e' - Move up.\n");
	printf("'q' - Move down.\n");
	printf("'ESC' - Quit the application.\n");

    glutMainLoop();
    return 0;
}

void init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    setupWindow(gameMode);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera->set();
    obj1->draw();
    obj2->draw();
    obj3->draw();

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

void setupWindow(bool gameMode) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    if (gameMode)
        currentWindow = glutEnterGameMode();
    else {
        glutInitWindowSize(width, height);
        glutInitWindowPosition(10, 10);
        currentWindow = glutCreateWindow("Team 1 - Assignment 3");
    }

    UserInput::init(*camera, speed, sensitivity, width, height);
    glutIgnoreKeyRepeat(1);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutSpecialFunc(onSpecialKey);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glEnable(GL_DEPTH_TEST); // TODO I'll enable it when I introduce textures
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