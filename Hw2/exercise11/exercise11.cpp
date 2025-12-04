#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"

void display();
void mouseHandler(int button, int state, int x, int y);
void keyboardHandler(unsigned char key, int x, int y);
void windowToWorldCoord(int *x, int *y);
void cleanUp();

RGB curColor = {.red = 1, .green = 0, .blue = 0};

void init() {
    // backround colour black, alpha parameter set to default
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(501, 501);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 2 - Exercise 9");

    // Set up handlers
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    printf("Keyboard commands:\n");
	printf("LMB - Select each vertex.\n");
	printf("RMB (while selecting polygon) - Select last vertex.\n");
	printf("RMB - Clear the board.\n");
	printf("'r' - Select red color for next vertex.\n");
	printf("'g' - Select green color for next vertex.\n");
	printf("'b' - Select blue color for next vertex.\n");
	printf("'c' - Select cyan color for next vertex.\n");
	printf("'y' - Select yellow color for next vertex.\n");
	printf("'m' - Select magenta color for next vertex.\n");
	printf("'q' - Quit the application.\n");

    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < Polygon::getTotalPolygons(); i++) {
        Polygon& p = Polygon::getPolygon(i);

        p.draw();
    }

    glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    Point vertex = {.x = localX, .y = localY, .rgb = curColor};

    if (button == GLUT_LEFT_BUTTON) {
        Polygon *poly = Polygon::getCurrentOrCreate();
        poly->addVertex(vertex);
    } else if (button == GLUT_RIGHT_BUTTON) {
        Polygon *poly = Polygon::getCurrent();
        if (poly != NULL)
            poly->addLastVertex(vertex);
        else
            Polygon::clear();
    }

    glutPostRedisplay();
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
        case 'R':
            curColor.red = 1.0f;
            curColor.green = 0.0f;
            curColor.blue = 0.0f;
            break;
        case 'g':
        case 'G':
            curColor.red = 0.0f;
            curColor.green = 1.0f;
            curColor.blue = 0.0f;
            break;
        case 'b':
        case 'B':
            curColor.red = 0.0f;
            curColor.green = 0.0f;
            curColor.blue = 1.0f;
            break;
        case 'm':
        case 'M':
            curColor.red = 1.0f;
            curColor.green = 0.0f;
            curColor.blue = 1.0f;
            break;
        case 'y':
        case 'Y':
            curColor.red = 1.0f;
            curColor.green = 1.0f;
            curColor.blue = 0.0f;
            break;
        case 'c':
        case 'C':
            curColor.red = 0.0f;
            curColor.green = 1.0f;
            curColor.blue = 1.0f;
            break;
        case 'Q':
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
}

void windowToWorldCoord(int *x, int *y) {
    float height = glutGet(GLUT_WINDOW_HEIGHT);

    *y = height - *y;
}