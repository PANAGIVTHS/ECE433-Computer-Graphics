#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "Camera.h"

const GLfloat speed = 0.2f;
const GLdouble fov = 90.0f;
const int width = 800;
const int height = 600;
const GLdouble aspect = (GLdouble) width / height;
Object *obj1 = new Object(-2.5f, 2.5f, -2.0f);
Object *obj2 = new Object(1.0f, 2.5f, -4.0f);
Object *obj3 = new Object(4.5f, 2.5f, -3.0f);
Camera *camera = new Camera(0.0f, 0.0f, 2.0f);

void init(int argc, char *argv[]);
void display();
void keyboardHandler(unsigned char key, int x, int y);
void windowToWorldCoord(int *x, int *y);

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 3");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, 1, 10);
    glMatrixMode(GL_MODELVIEW);
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

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            camera->move(FRONT, speed);
            break; 
        case 'a':
        case 'A':
            camera->move(LEFT, speed);
            break;    
        case 's':
        case 'S':
            camera->move(BACK, speed);
            break;
        case 'd':
        case 'D':
            camera->move(RIGHT, speed);
            break;
        case 'e':
        case 'E':
            camera->offset(0.0f, speed, 0.0f);
            break;
        case 'q':
        case 'Q':
            camera->offset(0.0f, -speed, 0.0f);
            break;
        case 'l':
        case 'L':
            camera->offsetCenter(speed, 0.0f, 0.0f);
            break;
        case 'j':
        case 'J':
            camera->offsetCenter(-speed, 0.0f, 0.0f);
            break;
        case 'i':
        case 'I':
            camera->offsetCenter(0.0f, speed, 0.0f);
            break;
        case 'k':
        case 'K':
            camera->offsetCenter(0.0f, -speed, 0.0f);
            break;
        case 27: // Escape
            exit(0);
        default:
            break;
    }

    glutPostRedisplay();
}

void windowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}