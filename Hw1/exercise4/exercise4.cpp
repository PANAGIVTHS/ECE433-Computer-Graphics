#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 1000

typedef struct {
    int x, y;
    float red, green, blue;
} Point;

typedef struct {
    Point start;
    Point end;
} Line;

Line lines[MAX_LINES];
Point curColor;
Line tempLine;

int lineCount = 0, isSecClick = -1, reshapeState = 0;
int newLineIdx = 0;

void windowToWorldCoord(int *x, int *y) {
    float halfWidth  = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

int sign(int number) {
    return (number > 0) - (number < 0);
}

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

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    
    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick == 1) {
            lines[newLineIdx].start.x = tempLine.start.x;
            lines[newLineIdx].start.y = tempLine.start.y;
            lines[newLineIdx].start.red = tempLine.start.red;
            lines[newLineIdx].start.green = tempLine.start.green;
            lines[newLineIdx].start.blue = tempLine.start.blue;
            lines[newLineIdx].end.x = localX;
            lines[newLineIdx].end.y = localY;
            lines[newLineIdx].end.red = curColor.red;
            lines[newLineIdx].end.green = curColor.green;
            lines[newLineIdx].end.blue = curColor.blue;
            isSecClick = 0;
            if (lineCount != MAX_LINES) lineCount++;
            newLineIdx = (newLineIdx + 1) % MAX_LINES;
            glutPostRedisplay();
        } else {
            //! Incase someone doesnt acutally go through with creating the line
            //! Dont overwrite previous' lines data when on max capacity
            tempLine.start.x = localX;
            tempLine.start.y = localY;
            tempLine.start.red = curColor.red;
            tempLine.start.green = curColor.green;
            tempLine.start.blue = curColor.blue;
            isSecClick = 1;
        }
    } else {
        lineCount = 0;
        newLineIdx = 0;
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
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

void drawLine(Point startPoint, Point endPoint) {
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
    
    //? Update draw buffer every line instead of pixel
    //? This is faster and per pixel makes 0 sense
    glBegin(GL_POINTS); 
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
            glVertex2i(leadingAxis, trailingAxis);
        } else {
            glVertex2i(trailingAxis, leadingAxis);
        }
    }
    glEnd();
}

void display(void) {
    //! Avoid initial call when window is created
    if (isSecClick == -1) return;
    
    if (reshapeState == 1) {
        for (int i = 0; i < lineCount; i++) {
            //! if you want to keep precedence start from 
            //! newLineIdx and move circularly instead of i
            drawLine(lines[i].start, lines[i].end);
        }
        reshapeState = 0;
    } else {
        int curLine = newLineIdx == 0 ? lineCount - 1 : newLineIdx - 1;
        drawLine(lines[curLine].start, lines[curLine].end);
    }
    glFlush();
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
        case 'Q':
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
}

void reshapeHandler(int w, int h) {
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    reshapeState = 1; //! Notify display to redraw ALL lines
    glutPostRedisplay();
}

// TODO Figure out if the color is meant to reset to red every new line
// TODO or just the first time the program starts??
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
    glutReshapeFunc(reshapeHandler);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
}