#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    int x, y;
} Point;

typedef struct {
    Point start, end;
} clippingWindow;

clippingWindow window;

// a flag that when is 1 it means that we draw a polygon
int polygon_draw = 0;
int clippingChosen = 0;

void init() {
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
    int localX = x, localY = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        windowToWorldCoord(&localX, &localY);
        window.start.x = localX;
        window.start.y = localY;
        drawPoint(window.start.x, window.start.y);
    }
    /*else {
        
    }
    */
    glFlush();
}

void mouseMove(int x, int y) {
    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    window.end.x = localX;
    window.end.y = localY;
    clippingChosen = 1;    
    glutPostRedisplay();
}


void keyboardHandler(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'GLUT_KEY_F1':
        break;
    case 'r':
    case 'R':
        //edw kaleitai mallon gia to cliping
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
	if (clippingChosen == 1) {
		glRecti(window.start.x, window.start.y, window.end.x, window.end.y);
    }
    glFlush();
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

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
    return(0);
}