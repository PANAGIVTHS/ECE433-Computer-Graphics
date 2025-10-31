#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
    float red, green, blue;
} Point;

Point startPoint, endPoint, curColor;
int isSecClick = 0;

// Sign function
int sign(int number) {
    return (number > 0) - (number < 0);
}

// Max function
int max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

void drawPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; // Ignore mouse release

    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick) {
            endPoint.x = x;
            endPoint.y = y;
            endPoint.red = curColor.red;
            endPoint.green = curColor.green;
            endPoint.blue = curColor.blue;
        } else {
            startPoint.x = x;
            startPoint.y = y;
            endPoint.x = x;
            endPoint.y = y;
            startPoint.red = curColor.red;
            startPoint.green = curColor.green;
            startPoint.blue = curColor.blue;
        }

        isSecClick = !isSecClick;
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glutPostRedisplay();
}

void init() {
    // background colour white, alpha parameter set to default
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();
    glPointSize(1.0f);
    curColor.red = 1.0f;
    curColor.green = 0.0f;
    curColor.blue = 0.0f;
	
    // viewing area 
    gluOrtho2D(-300, 300, -300, 300); 
}

void display(void) {
    Point delta; //! Dummy point to store delta values
    int condSlope = 0, error = 0;
    int leadingAxis = 0, trailingAxis = 0, endPointCoord = 0;
    int signLeadingAxis = 0, signTrailingAxis = 0,
        dstLeadingAxis = 0, dstTrailingAxis = 0;

    // Calculate axis distance 
    int dstX = abs(endPoint.x - startPoint.x);
    int dstY = abs(endPoint.y - startPoint.y);
    
    // Set default color
    glColor3f(startPoint.red, startPoint.green, startPoint.blue);
    
    // Starting point == End point
    if (!(dstX || dstY)) {
        drawPoint(startPoint.x, startPoint.y);
        return;
    }
    
    int numPixels = max(dstX, dstY);
    condSlope = dstX >= dstY;

    //! This can be u_int. Could be faster and more efficient 
    delta.red = (endPoint.red - startPoint.red) / numPixels;
    delta.green = (endPoint.green - startPoint.green) / numPixels;
    delta.blue = (endPoint.blue - startPoint.blue) / numPixels;

    // Pick leading and trailing axis
    if (condSlope) {
        // Initialise points
        leadingAxis = startPoint.x;
        trailingAxis = startPoint.y;
        endPointCoord = endPoint.x;
        
        // Initialise axis variables
        signLeadingAxis = sign(endPoint.x - startPoint.x);
        signTrailingAxis = sign(endPoint.y - startPoint.y);
        dstLeadingAxis = dstX;
        dstTrailingAxis = dstY;
        
        // Draw starting point
        drawPoint(leadingAxis, trailingAxis);
    } else {
        // Initialise points
        leadingAxis = startPoint.y;
        trailingAxis = startPoint.x;
        endPointCoord = endPoint.y;
        
        // Initialise axis variables
        signLeadingAxis = sign(endPoint.y - startPoint.y);
        signTrailingAxis = sign(endPoint.x - startPoint.x);
        dstLeadingAxis = dstY;
        dstTrailingAxis = dstX;
        
        // Draw starting point
        drawPoint(trailingAxis, leadingAxis);
    }

    // Initialise error variable
    error = (2 * dstTrailingAxis) - dstLeadingAxis;

    while (leadingAxis != endPointCoord) {
        error += 2 * dstTrailingAxis;
        leadingAxis += signLeadingAxis;
    
        if (error >= 0) {
            error -= 2 * dstLeadingAxis;
            trailingAxis += signTrailingAxis;
        }
        
        // Draw pixel at specified point and increment deltas
        startPoint.red += delta.red;
        startPoint.green += delta.green;
        startPoint.blue += delta.blue;
        glColor3f(startPoint.red, startPoint.green, startPoint.blue);
        if (condSlope) {
            drawPoint(leadingAxis, trailingAxis);
        } else {
            drawPoint(trailingAxis, leadingAxis);
        }
    }
}

// function that terminates the program with the press of button Q/q
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
    
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 1 - Exercise 4"); 

    // Set up handlers
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);

    init();                                          // additional initializations
    glutMainLoop();
}