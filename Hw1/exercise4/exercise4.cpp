#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

float startPointRGB[3], endPointRGB[3];

typedef enum {
    RED = 0,
    GREEN = 1,
    BLUE = 2
} RGB;

// Sign function
int sign(int number) {
    return (number > 0) - (number < 0);
}

// Max function
int max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

void init() {
    // background colour white, alpha parameter set to default
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();    
	
    // viewing area 
    gluOrtho2D(-300, 300, -300, 300); 
}

void display(int xP0, int yP0, int xP1, int yP1) {
    float delta[3] = {0};
    int condSlope = 0, error = 0;
    int leadingAxis = 0, trailingAxis = 0, endPointCoord = 0;
    int signLeadingAxis = 0, signTrailingAxis = 0,
        dstLeadingAxis = 0, dstTrailingAxis = 0;

    float rVal = startPointRGB[RED];
    float gVal = startPointRGB[GREEN];
    float bVal = startPointRGB[BLUE];
    
    // Calculate axis distance 
    int dstX = abs(xP1 - xP0);
    int dstY = abs(yP1 - yP0);
    
    // If P0 = P1
    if (!(dstX && dstY)) {
        glBegin(GL_POINTS);
        glVertex2i(xP0, yP0);
        glEnd();
        return;
    }
    
    int numPixels = max(dstX, dstY);
    condSlope = dstX >= dstY;

    //! This can be u_int. Could be faster and more efficient 
    delta[RED] = (endPointRGB[RED] - startPointRGB[RED]) / numPixels;
    delta[GREEN] = (endPointRGB[GREEN] - startPointRGB[GREEN]) / numPixels;
    delta[BLUE] = (endPointRGB[BLUE] - startPointRGB[BLUE]) / numPixels;

    // Pick leading and trailing axis
    if (condSlope) {
        // Initialise points
        leadingAxis = xP0;
        trailingAxis = yP0;
        endPointCoord = xP1;
        
        // Initialise axis variables
        signLeadingAxis = sign(xP1 - xP0);
        signTrailingAxis = sign(yP1 - yP0);
        dstLeadingAxis = dstX;
        dstTrailingAxis = dstY;
        
        // Draw starting point
        glBegin(GL_POINTS);
        glVertex2i(leadingAxis, trailingAxis);
        glEnd();
    } else {
        // Initialise points
        leadingAxis = yP0;
        trailingAxis = xP0;
        endPointCoord = yP1;
        
        // Initialise axis variables
        signLeadingAxis = sign(yP1 - yP0);
        signTrailingAxis = sign(xP1 - xP0);
        dstLeadingAxis = dstY;
        dstTrailingAxis = dstX;
        
        // Draw starting point
        glBegin(GL_POINTS);
        glVertex2i(trailingAxis, leadingAxis);
        glEnd();
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
        glBegin(GL_POINTS); 
        rVal += delta[RED];
        gVal += delta[GREEN];
        bVal += delta[BLUE];
        glColor3f(rVal, gVal, bVal);
        if (condSlope) {
            glVertex2i(leadingAxis, trailingAxis);
        } else {
            glVertex2i(trailingAxis, leadingAxis);
        }
        glEnd();
    }
}

// function that terminates the program with the press of button Q/q
void termination (unsigned char key, int x, int y) {
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
    
    // set window size
    glutInitWindowSize(600, 600);

    // set window initial position
    glutInitWindowPosition(10, 10);
    
    // create window with the given title
    glutCreateWindow("Team 1 - Assignment 1 - Exercise 4"); 

    glutDisplayFunc();

}