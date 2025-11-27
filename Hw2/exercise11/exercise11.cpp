#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void display();
void mouseHandler(int button, int state, int x, int y);
void keyboardHandler(unsigned char key, int x, int y);
void windowToWorldCoord(int *x, int *y);
void cleanUp();

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

class Polygon {
    private:
        static Polygon **polys;
        static int totalPolys;
        static bool selectingPolygon;

        Point *vertices = NULL;
        int totalVertices = 0;

        static Polygon * addPolygon() {
            polys = (Polygon **) realloc(polys, (++totalPolys) * sizeof(Polygon *));
            Polygon *newPolygon = (Polygon *) malloc(sizeof(Polygon));
            polys[totalPolys - 1] = newPolygon;
        }

    public:
        ~Polygon() {
            free(vertices);
        }

        void addVertex(Point point) {
            vertices = (Point *) realloc(vertices, (++totalVertices) * sizeof(Point));
            vertices[totalVertices - 1] = point;
        }

        static void init() {
            polys = (Polygon **) malloc(sizeof(Polygon *));
            totalPolys = 0;
            selectingPolygon = false;
        }

        static void destroy() {
            for (int i = 0; i < totalPolys; i++) {
                Polygon *poly = polys[i];
                poly->~Polygon();
                free(poly);
            }
            free(polys);
        }

        static Polygon * getCurrent() {
            return selectingPolygon ? polys[totalPolys - 1] : NULL;
        }

        static Polygon * getCurrentOrCreate() {
            return selectingPolygon ? polys[totalPolys - 1] : addPolygon();
        }

        static bool completeCurrent() {
            bool oldVal = selectingPolygon;
            selectingPolygon = false;
            return oldVal;
        }
};

RGB curColor = {.red = 1, .green = 0, .blue = 0};

void init() {
    // initialize polygon array
    Polygon::init();

    // backround colour black, alpha parameter set to default
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1.0f);

    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(501, 501);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Team 1 - Assignment 2 - Exercise 9");

    // Set up handlers
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);

    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutMainLoop();
}

void display() {

}

void mouseHandler(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return; //! Ignore mouse release

    int localX = x, localY = y;
    windowToWorldCoord(&localX, &localY);

    if (button == GLUT_LEFT_BUTTON) {
        Point vertex = {.x = localX, .y = localY, .rgb = curColor};
        Polygon *poly = Polygon::getCurrentOrCreate();
        poly->addVertex(vertex);
    } else if (button == GLUT_RIGHT_BUTTON && !Polygon::completeCurrent()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
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
        case 'm':
        case 'M':
            curColor.red = 1.0f;
            curColor.green = 0.0f;
            curColor.blue = 1.0f;
            break;
        case 'y':
        case 'Y':
            curColor.red = 1.0f;
            curColor.green = 1.0f;
            curColor.blue = 0.0f;
            break;
        case 'c':
        case 'C':
            curColor.red = 0.0f;
            curColor.green = 1.0f;
            curColor.blue = 1.0f;
            break;
        case 'Q':
        case 'q':
            cleanUp();
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