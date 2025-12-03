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
    // initialize polygon array
    Polygon::init();

    // backround colour black, alpha parameter set to default
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 2 - Exercise 9");

    // Set up handlers
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
}

void display() {
    if (Polygon::getTotalPolygons() == 0) { 
        // // 1. TOP VERTEX (Red)
        // // Window Coord: (250, 150) -> World Coord: (0, 100)
        // // Set your color to RED here (e.g., currentColor = {1, 0, 0};)
        // curColor = {.red=0,.green=0,.blue=1};
        // mouseHandler(GLUT_LEFT_BUTTON, GLUT_DOWN, 250, 150);
        // curColor = {.red=1,.green=0,.blue=0};

        // // 2. BOTTOM LEFT VERTEX (Green)
        // // Window Coord: (150, 350) -> World Coord: (-100, -100)
        // // Set your color to GREEN here
        // mouseHandler(GLUT_LEFT_BUTTON, GLUT_DOWN, 150, 350);
        // curColor = {.red=0,.green=1,.blue=0};

        // // 3. BOTTOM RIGHT VERTEX (Blue)
        // // Window Coord: (350, 350) -> World Coord: (100, -100)
        // // Set your color to BLUE here
        // mouseHandler(GLUT_RIGHT_BUTTON, GLUT_DOWN, 350, 350);

        return;
    }

    Polygon *poly = Polygon::getCurrent();
    poly->drawLastVertex();

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
        glutPostRedisplay();
    } else if (button == GLUT_RIGHT_BUTTON && !Polygon::completeCurrent(vertex)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
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
            cleanUp();
            exit(0);
            break;
        default:
            break;
    }
}

void windowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

void cleanUp() {
    Polygon::destroy();
}