#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_OBJECTS 3000

#define MAKE_OFST_POINT(base, dx, dy, col) ((Point){ (base).x + (dx), (base).y + (dy), (col) })

typedef enum {
    OBJ_POINT,
    OBJ_LINE,
    OBJ_CIRCLE,
    OBJ_FCIRCLE
} ObjectType;

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

typedef struct {
    Point center;
    int radius;
} Circle;

typedef struct {
    Point center;
    RGB secondary;
    int radius;
} FilledCircle;

typedef union {
    Point point;
    Line line;
    Circle circle;
    FilledCircle fcircle;
} ObjectData;

typedef struct {
    ObjectType type;
    ObjectData data;
} Object;

Object objects[MAX_OBJECTS];

RGB curColor;
Object *curObject;
ObjectType typeSelector = OBJ_CIRCLE;
Point firstMousePoint, secondMousePoint; 

int objCount = 0, newObjIndex = 0, reshapeState = 0;
int isSecClick = -1;

void storeObj(Object object);
Object newObj(ObjectType type);
void drawObject(Object object);

//! Utility functions
int max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

int sign(int number) {
    return (number > 0) - (number < 0);
}

void windowToWorldCoord(int *x, int *y) {
    float halfWidth = glutGet(GLUT_WINDOW_WIDTH) / 2.0f;
    float halfHeight = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    *x -= halfWidth;
    *y = halfHeight - *y;
}

//! Draw function calls
void drawPoint(Point point) {
    glBegin(GL_POINTS);
    glColor3f(point.rgb.red, point.rgb.green, point.rgb.blue);
    glVertex2i(point.x, point.y);
    glEnd();
    glFlush();
}

void drawLine(Line line) {
    int error = 0;
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
        drawPoint(startPoint);
        return;
    }
    
    int condSlope = dstX >= dstY;
    int numPixels = max(dstX, dstY);

    RGB delta = {
        (line.end.rgb.red - line.start.rgb.red) / numPixels,
        (line.end.rgb.green - line.start.rgb.green) / numPixels,
        (line.end.rgb.blue - line.start.rgb.blue) / numPixels
    };
    
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
    }

    glBegin(GL_POINTS);

    // Initialise error variable
    error = (2 * dstTrailingAxis) - dstLeadingAxis;
    RGB color = startPoint.rgb;  // use local color copy
    
    //? Update draw buffer every line instead of pixel
    //? This is faster and per pixel makes 0 sense
    for (int i = 0; i <= numPixels; ++i) {  // loop over exact number of pixels
        glColor3f(startPoint.rgb.red, startPoint.rgb.green, startPoint.rgb.blue);
        if (condSlope) {
            glVertex2i(leadingAxis, trailingAxis);
        } else {
            glVertex2i(trailingAxis, leadingAxis);
        }

        leadingAxis += signLeadingAxis;
        if (error >= 0) {
            error -= 2 * dstLeadingAxis;
            trailingAxis += signTrailingAxis;
        }
        error += 2 * dstTrailingAxis;

        startPoint.rgb.red += delta.red;
        startPoint.rgb.green += delta.green;
        startPoint.rgb.blue += delta.blue;
        
    }
    glEnd();
}

void drawCircle(Circle circle) {
    Point centerPoint = circle.center;
    int radius = circle.radius;
    int xOffset = 0, yOffset = radius;
    int error = 3 - (2 * radius);

    glBegin(GL_POINTS);
    glColor3f(centerPoint.rgb.red, centerPoint.rgb.green, centerPoint.rgb.blue);
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

void drawCircleFilled(FilledCircle fcircle) {
    Point c = fcircle.center;
    int radius = fcircle.radius;
    int xOffset = 0, yOffset = radius;
    int error = 3 - (2 * radius);

    RGB endRGB = fcircle.secondary;
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

void redrawAll() {
    for (int i = 0; i < objCount; i++) {
        drawObject(objects[i]);
    }
}

//! Object handling
void storeObj(Object object) {
    objects[newObjIndex] = object;
    if (objCount != MAX_OBJECTS) objCount++;
    newObjIndex = (newObjIndex + 1) % MAX_OBJECTS;
    int current = newObjIndex == 0 ? objCount - 1 : newObjIndex - 1;
    curObject = &objects[current];
}

Object newObj(ObjectType type) {
    Object object;

    object.type = type;
    switch(type) {
        case OBJ_POINT: {
            object.data.point = firstMousePoint;
            break;
        }
        case OBJ_LINE: {
            object.data.line.start = firstMousePoint;
            object.data.line.end = secondMousePoint;
            break;
        }
        case OBJ_CIRCLE: {
            int dstX = secondMousePoint.x - firstMousePoint.x;
            int dstY = secondMousePoint.y - firstMousePoint.y;
            object.data.circle.radius = sqrt((dstX * dstX) + (dstY * dstY));
            object.data.circle.center = firstMousePoint;
            break;
        }
        case OBJ_FCIRCLE: {
            int dstX = secondMousePoint.x - firstMousePoint.x;
            int dstY = secondMousePoint.y - firstMousePoint.y;
            object.data.fcircle.radius = sqrt((dstX * dstX) + (dstY * dstY));
            object.data.fcircle.center = firstMousePoint;
            object.data.fcircle.secondary = secondMousePoint.rgb;
            break;
        }
        default: {
            int dstX = secondMousePoint.x - firstMousePoint.x;
            int dstY = secondMousePoint.y - firstMousePoint.y;
            object.data.circle.radius = sqrt((dstX * dstX) + (dstY * dstY));
            object.data.circle.center = firstMousePoint;
            break;
        }
    }

    return(object);
}

void drawObject(Object object) {
    switch(object.type) {
        case OBJ_POINT: {
            drawPoint(object.data.point);
            break;
        }
        case OBJ_LINE: {
            drawLine(object.data.line);
            break;
        }
        case OBJ_CIRCLE: {
            drawCircle(object.data.circle);
            break;
        }
        case OBJ_FCIRCLE: {
            drawCircleFilled(object.data.fcircle);
            break;
        }
        default: {
            drawCircle(object.data.circle);
            break;
        }
    }
}

//! OpenGL calls
void display() {
    if (isSecClick == -1) return;
    
    if (reshapeState == 1) {
        //! Redraw all object relative to new window
        redrawAll();
        reshapeState = 0;
    } else {
        drawObject(*curObject);
    }
    glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);

    if (button == GLUT_LEFT_BUTTON) {
        if (isSecClick == 1) {
            secondMousePoint.x = localX;
            secondMousePoint.y = localY;
            secondMousePoint.rgb.red = curColor.red;
            secondMousePoint.rgb.green = curColor.green;
            secondMousePoint.rgb.blue = curColor.blue;
            
            //! Create new object from global point data
            //! and add to global memory
            storeObj(newObj(typeSelector)); 
            
            isSecClick = 0;
            glutPostRedisplay();
        } else {
            firstMousePoint.x = localX;
            firstMousePoint.y = localY;
            firstMousePoint.rgb.red = curColor.red;
            firstMousePoint.rgb.green = curColor.green;
            firstMousePoint.rgb.blue = curColor.blue;
            isSecClick = 1;
        }
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
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

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
        case 'P':
            typeSelector = OBJ_POINT;
            break;
        case 'l':
        case 'L':
            typeSelector = OBJ_LINE;
            break;
        case 'c':
        case 'C':
            typeSelector = OBJ_CIRCLE;
            break;
        case 'f':
        case 'F':
            typeSelector = OBJ_FCIRCLE;
            break;
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
    glutReshapeFunc(reshapeHandler);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
}