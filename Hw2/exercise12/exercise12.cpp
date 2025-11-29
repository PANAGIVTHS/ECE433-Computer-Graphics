#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"

typedef enum {
    CLIPPING_WINDOW,
    POLYGON_DRAWING
} State;

typedef struct {
    Point start, end;
} clippingWindow;

clippingWindow window;
State curState = POLYGON_DRAWING;

/*
a flag that when it's 1 it means that we draw a polygon
int polygonChosen = 0;
a flag that when it's 1 it means that we draw the clipping window
int clippingChosen = 0;
*/

void init() {
    //initialize polygon array
    Polygon::init();
    
    //backround colour dark grey, alpha parameter set to default
    glClearColor(0.2, 0.2, 0.2, 0.0);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-300, 300, -300, 300);
}

void drawPoint(int x, int y) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(1.0f);
    glBegin(GL_POINT);
        glVertex2i(x, y);
    glEnd();
    glFlush();
}

void windowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}


void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;
    
    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
            
    // checking if we are in the state that we draw the polygons
    if (curState == POLYGON_DRAWING) {
        
        Point vertex = {.x = localX, .y = localY};
        
        if (button == GLUT_LEFT_BUTTON) {
            Polygon *poly = Polygon::getCurrentOrCreate();
            poly->addVertex(vertex);
            glutPostRedisplay();
        }
        else if (button == GLUT_RIGHT_BUTTON && Polygon::completeCurrent(vertex)) {
            glutPostRedisplay();
        }    
        /*ETSI OPWS DIABAZW THN EKFWNHSH DEN EXW KATALABEI AN LEEI GIA KAPOIO
        CLEAR TIS OTHONIS ME TO RIGHT BUTTON, ISWS AYTO XREIASTEI GIA TO CLIPPING WINDOW IDK*/
    }
    else if (curState == CLIPPING_WINDOW) {
        if (button == GLUT_LEFT_BUTTON) {
            window.start.x = localX;
            window.start.y = localY;
            drawPoint(window.start.x, window.start.y);
        }
    }
    glFlush();
}

void mouseMove(int x, int y) {
    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    if (curState == CLIPPING_WINDOW) {
        window.end.x = localX;
        window.end.y = localY;
        glutPostRedisplay();
    }
}

void specialKeyHandler(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        if (curState == POLYGON_DRAWING) {
            curState = CLIPPING_WINDOW;
        }
        else {
            curState = POLYGON_DRAWING;
        }
        break;
    }
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

void display(){
	glClear(GL_COLOR_BUFFER_BIT);	//The buffers currently enabled for color writing
	glColor3f(1.0, 1.0, 1.0); 
	if (curState == CLIPPING_WINDOW) {
		glRecti(window.start.x, window.start.y, window.end.x, window.end.y);
    }
    else if(curState == POLYGON_DRAWING) {
        Polygon *poly = Polygon::getCurrent();
        if (poly == NULL) return;
        
        poly->draw();
    }
    glFlush();
}

void cleanUp() {
    Polygon::destroy();
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