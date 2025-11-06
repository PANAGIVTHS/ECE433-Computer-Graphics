#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 1000

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

typedef struct {
    Point start;
    Point end;
} Line;

Line lines[MAX_LINES];
RGB curColor;
Line tempLine;
Line *newestLine;

int lineCount = 0, isSecClick = -1, reshapeState = 0;
int newLineIdx = 0;

int max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

int sign(int number) {
    return (number > 0) - (number < 0);
}

void windowToWorldCoord(int *x, int *y) {
    float halfWidth  = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

void drawPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void storeLine(Line line) {
    lines[newLineIdx] = line;
    if (lineCount != MAX_LINES) lineCount++;
    newLineIdx = (newLineIdx + 1) % MAX_LINES;
    int curLine = newLineIdx == 0 ? lineCount - 1 : newLineIdx - 1;
    newestLine = &lines[curLine];
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);
    
    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick == 1) {
            tempLine.end.x = localX;
            tempLine.end.y = localY;
            tempLine.end.rgb.red = curColor.red;
            tempLine.end.rgb.green = curColor.green;
            tempLine.end.rgb.blue = curColor.blue;
            isSecClick = 0;
            storeLine(tempLine);
            glutPostRedisplay();
        } else {
            //! Incase someone doesnt acutally go through with creating the line
            //! Dont overwrite previous' lines data when on max capacity
            tempLine.start.x = localX;
            tempLine.start.y = localY;
            tempLine.start.rgb.red = curColor.red;
            tempLine.start.rgb.green = curColor.green;
            tempLine.start.rgb.blue = curColor.blue;
            isSecClick = 1;
        }
    } else {
        lineCount = 0;
        newLineIdx = 0;
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}

void drawLine(Line line) {
    RGB delta;
    int condSlope = 0, error = 0;
    int leadingAxis = 0, trailingAxis = 0, endPointCoord = 0;
    int signLeadingAxis = 0, signTrailingAxis = 0,
        dstLeadingAxis = 0, dstTrailingAxis = 0;
    
    Point endPoint = line.end;
    Point startPoint = line.start;

    // Calculate axis distance 
    int dstX = abs(endPoint.x - startPoint.x);
    int dstY = abs(endPoint.y - startPoint.y);
    
    // Set default color
    glColor3f(startPoint.rgb.red, startPoint.rgb.green, startPoint.rgb.blue);
    
    // Starting point == End point
    if (!(dstX || dstY)) {
        drawPoint(startPoint.x, startPoint.y);
        return;
    }
    
    int numPixels = max(dstX, dstY);
    condSlope = dstX >= dstY;

    delta.red = (endPoint.rgb.red - startPoint.rgb.red) / numPixels;
    delta.green = (endPoint.rgb.green - startPoint.rgb.green) / numPixels;
    delta.blue = (endPoint.rgb.blue - startPoint.rgb.blue) / numPixels;

    glBegin(GL_POINTS); 
    glColor3f(startPoint.rgb.red, startPoint.rgb.green, startPoint.rgb.blue);
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
        glVertex2i(leadingAxis, trailingAxis);
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
        glVertex2i(trailingAxis, leadingAxis);
    }

    // Initialise error variable
    error = (2 * dstTrailingAxis) - dstLeadingAxis;
    
    //? Update draw buffer every line instead of pixel
    //? This is faster and per pixel makes 0 sense
    while (leadingAxis != endPointCoord) {
        leadingAxis += signLeadingAxis;
        
        if (error >= 0) {
            error -= 2 * dstLeadingAxis;
            trailingAxis += signTrailingAxis;
        }
        error += 2 * dstTrailingAxis;
        
        // Draw pixel at specified point and increment deltas
        startPoint.rgb.red += delta.red;
        startPoint.rgb.green += delta.green;
        startPoint.rgb.blue += delta.blue;
        glColor3f(startPoint.rgb.red, startPoint.rgb.green, startPoint.rgb.blue);
        if (condSlope) {
            glVertex2i(leadingAxis, trailingAxis);
        } else {
            glVertex2i(trailingAxis, leadingAxis);
        }
    }
    glEnd();
}

void redrawAll() {
    for (int i = 0; i < lineCount; i++) {
        //! if you want to keep precedence start from 
        //! newLineIdx and move circularly instead of i
        drawLine(lines[i]);
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

void display(void) {
    //! Avoid initial call when window is created
    if (isSecClick == -1) return;
    
    if (reshapeState == 1) {
        //! Redraw all object relative to new window
        redrawAll();
        reshapeState = 0;
    } else {
        drawLine(*newestLine);
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