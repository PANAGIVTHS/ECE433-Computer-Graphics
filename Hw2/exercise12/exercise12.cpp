#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"
#include "clippingWindow.h"

typedef enum {
    CLIPPING_WINDOW,
    POLYGON_DRAWING
} SelectingState;

void display();
void mouseMove(int x, int y);
void mouseHandler(int button, int state, int x, int y);
void keyboardHandler(unsigned char key, int x, int y);
void windowToWorldCoord(int *x, int *y);
void cleanUp();

RGB curColor = {.red = 1, .green = 0, .blue = 1};
SelectingState curState = POLYGON_DRAWING;
ClippingWindow window;

void init() {
    //initialize polygon array
    Polygon::init();
    
    //backround colour dark grey, alpha parameter set to default
    glClearColor(0.4, 0.4, 0.4, 0.0);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-300, 300, -300, 300);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 2 - Exercise 12");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutMotionFunc(mouseMove);
    glutSpecialFunc(specialKeyHandler);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
    return(0);
}

void mouseMove(int x, int y) {
    if (curState != CLIPPING_WINDOW) return;

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    window.setEndPoint(localX, localY);
    glutPostRedisplay();
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);

    if (curState == CLIPPING_WINDOW) {
        window.setStartPoint(localX, localY);
        glutPostRedisplay();
    }

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

void specialKeyHandler(int key, int x, int y) {
    if (key != GLUT_KEY_F1) return;
    
    if (curState == POLYGON_DRAWING)
        curState = CLIPPING_WINDOW;
    else
        curState = POLYGON_DRAWING;
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
        case 'R':
            //edw kaleitai to cliping
            break;    
        case 'c':
        case 'C':
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
            break;
        case 'q':
        case 'Q':
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