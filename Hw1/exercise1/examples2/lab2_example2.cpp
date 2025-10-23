// Eisagwgh shmeiwn kai kaqarismos oqonhs me to pontiki
// H entolh glutMouseFunc()

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

int screenWidth = 640;
int screenHeight = 480;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);              // select clearing (background) color
    glColor3f(1.0, 1.0, 1.0);                      // initialize drawing color
    glPointSize(4);                                // set point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);                  // clear the color buffer

    glBegin(GL_LINES);
        glVertex2i(100, 50);
        glVertex2i(100, 150);
        glVertex2i(150, 150);
        glVertex2i(250, 250);
        glVertex2i(550, 350);
        glVertex2i(650, 450);
    glEnd();

    glFlush();                                     // force execution of GL commands
}

// triggered when a keyboard key is pressed
void keyEvent(unsigned char key, int x, int y) {   // x, y = mouse location when key pressed
    switch (key) {
        case 'Q':
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
}

void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
            glVertex2i(x, screenHeight - y);
        glEnd();
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                         // initialize toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // set display mode
    glutInitWindowSize(640, 480);                  // set window size
    glutInitWindowPosition(1, 1);                  // set window initial position
    glutCreateWindow("mouseFunc1");                // create window

    glutDisplayFunc(display);                      // register display callback
    glutKeyboardFunc(keyEvent);                    // register keyboard callback
    glutMouseFunc(myMouse);                        // register mouse callback

    init();                                        // initialize settings
    glutMainLoop();                                // enter perpetual event loop
    return 0;
}
