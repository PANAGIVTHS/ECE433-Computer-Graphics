#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINES 1000

#define MAKE_OFST_POINT(base, dx, dy, col) ((Point){ (base).x + (dx), (base).y + (dy), (col) })

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
Point startPoint, endPoint; 

int lineCount = 0;
int newLineIdx = 0;
int isSecClick = -1;

int max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

int sign(int number) {
    return (number > 0) - (number < 0);
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
        error += 2 * dstTrailingAxis;
        leadingAxis += signLeadingAxis;
    
        if (error >= 0) {
            error -= 2 * dstLeadingAxis;
            trailingAxis += signTrailingAxis;
        }
        
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

void windowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);

    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick == 1) {
            endPoint.x = localX;
            endPoint.y = localY;
            isSecClick = 0;
            glutPostRedisplay();
        } else {
            startPoint.x = localX;
            startPoint.y = localY;
            startPoint.rgb.red = curColor.red;
            startPoint.rgb.green = curColor.green;
            startPoint.rgb.blue = curColor.blue;
            isSecClick = 1;
        }
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}

void drawCircle(Point centerPoint, int radius) {
    int xOffset = 0, yOffset = radius;
    int error = 3 - (2 * radius);

    glBegin(GL_POINTS);
    glColor3f(curColor.red, curColor.green, curColor.blue);
    while(xOffset <= yOffset) {
        int xPos = centerPoint.x + xOffset;
        int xNeg = centerPoint.x - xOffset;
        int yPos = centerPoint.y + yOffset;
        int yNeg = centerPoint.y - yOffset;
        int yxPos = centerPoint.x + yOffset;
        int yxNeg = centerPoint.x - yOffset;
        int xyPos = centerPoint.y + xOffset;
        int xyNeg = centerPoint.y - xOffset;

        glVertex2i(xPos,  yPos);
        glVertex2i(yxPos, xyPos);
        glVertex2i(xNeg,  yPos);
        glVertex2i(yxPos, xyNeg);
        glVertex2i(xNeg,  yNeg);
        glVertex2i(yxNeg, xyNeg);
        glVertex2i(xPos,  yNeg);
        glVertex2i(yxNeg, xyPos);
    
        xOffset++;
        if (error >= 0) {
            yOffset--;
            error -= 4 * yOffset;
        }
        error += (4 * xOffset) + 2;
    }
    glEnd();
    glFlush();
}

void drawCircleFilled(Point centerPoint, int radius) {
    int xOffset = 0, yOffset = radius;
    int error = 3 - (2 * radius);
    Point c = centerPoint;

    RGB endRGB = curColor;
    while(xOffset <= yOffset) {
        Point topLeft      = MAKE_OFST_POINT(c, -xOffset,  yOffset, c.rgb);
        Point topRight     = MAKE_OFST_POINT(c,  xOffset,  yOffset, endRGB);
        Point bottomLeft   = MAKE_OFST_POINT(c, -xOffset, -yOffset, c.rgb);
        Point bottomRight  = MAKE_OFST_POINT(c,  xOffset, -yOffset, endRGB);
        Point leftTop      = MAKE_OFST_POINT(c, -yOffset,  xOffset, c.rgb);
        Point rightTop     = MAKE_OFST_POINT(c,  yOffset,  xOffset, endRGB);
        Point leftBottom   = MAKE_OFST_POINT(c, -yOffset, -xOffset, c.rgb);
        Point rightBottom  = MAKE_OFST_POINT(c,  yOffset, -xOffset, endRGB);

        Line topSpan       = { topLeft, topRight };
        Line bottomSpan    = { bottomLeft, bottomRight };
        Line upperDiagSpan = { leftTop, rightTop };
        Line lowerDiagSpan = { leftBottom, rightBottom };

        drawLine(topSpan);
        drawLine(bottomSpan);
        drawLine(upperDiagSpan);
        drawLine(lowerDiagSpan);
        
        xOffset++;
        if (error >= 0) {
            yOffset--;
            error -= 4 * yOffset;
        }
        error += (4 * xOffset) + 2;
    }
}

void display() {
    if (isSecClick == -1) return;
    int dstX, dstY;
    
    dstX = endPoint.x - startPoint.x;
    dstY = endPoint.y - startPoint.y;

    int radius = sqrt((dstX * dstX) + (dstY * dstY));  
    drawCircleFilled(startPoint, radius);
    glFlush();
}

void init() {
    // backround colour black, alpha parameter set to default
    glClearColor(0.0, 0.0, 0.0, 0.0);
    curColor.red = 1.0f;
    curColor.green = 0.0f;
    curColor.blue = 0.0f;
    
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-400, 400, -400, 400);
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