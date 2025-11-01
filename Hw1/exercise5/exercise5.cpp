#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int xCord, yCord;
} Point;

typedef struct {
    int xRadius, yRadius;
    int radius;
} PointRadius;

Point firstCoord;
PointRadius secondCoord; 

int isSecClick = 0;

int toThePower2(int number) {
    return number * number;
}

void WindowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; // Ignore mouse release

    int localX = x, localY = y;
    //WindowToWorldCoord(&localX, &localY);

    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick == 1) {
            secondCoord.xRadius = localX;
            secondCoord.yRadius = localY;
            secondCoord.radius = 
            sqrt((toThePower2((abs(secondCoord.xRadius) - abs(firstCoord.xCord))))
            +(toThePower2((abs(secondCoord.yRadius) - abs(firstCoord.yCord)))));   
            isSecClick = 0;
            glutPostRedisplay();
        } 
        else {
            firstCoord.xCord = localX;
            firstCoord.yCord = localY;
            isSecClick = 1;
        }
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}
void drawCircle(int x, int y, int radius) {

    int x_center = 0, y_center = radius;
    int x_point = x, y_point = y;

    int error = 3 - (2 * radius);

    glBegin(GL_POINTS);
    while(x_center <= y_center) {
        glVertex2i(x_point + x_center, y_point + y_center); // (x,y) point
        glVertex2i(x_point + y_center, y_point + x_center); // (y,x) point

        glVertex2i(x_point - x_center, y_point + y_center); // (-x, y) point
        glVertex2i(x_point + y_center, y_point - x_center); // (y, -x) point

        glVertex2i(x_point - x_center, y_point - y_center); // (-x, -y) point
        glVertex2i(x_point - y_center, y_point - x_center); // (-y, -x) point

        glVertex2i(x_point + x_center, y_point - y_center); // (x, -y) point
        glVertex2i(x_point - y_center, y_point + x_center); // (-y, x) point
    
        x_center += 1;
        if (error >= 0) {
            y_center -= 1;
            error -= 4 * y_center;
        }
        error += (4 * x_center) + 2;
    }
    glEnd();
    glFlush();
}

void display() {
    drawCircle(firstCoord.xCord, firstCoord.yCord, secondCoord.radius);
}

void init() {

    // backround colour black, alpha parameter set to default
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-400, 400, -400, 400);
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'Q':
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 1 - Exercise 5");

    // Set up handlers
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);
    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
}