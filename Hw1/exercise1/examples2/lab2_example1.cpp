#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

GLfloat vertices[3][3] = {
    {70.0, 250.0, 0.0},
    {30.0, 350.0, 0.0},
    {10.0,  50.0, 0.0}
};

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);     // select clearing (background) color
    glColor3f(1.0, 0.0, 0.0);             // initialize drawing color
    glPointSize(4);                       // set point size
    // glShadeModel(GL_FLAT);
    // glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);   // define a 2-D orthographic projection matrix
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);         // clear color buffer

    // Choose one drawing mode
    // glBegin(GL_POINTS);
    glBegin(GL_LINES);
    // glBegin(GL_LINE_LOOP);
    // glBegin(GL_POLYGON);
    // glBegin(GL_TRIANGLES);

        glVertex2i(10, 10);
        glVertex2i(50, 90);

        glColor3f(0.0, 1.0, 0.0);
        glVertex2i(90, 10);

        glColor3f(1.0, 1.0, 0.0);
        glVertex2i(10, 70);
        glVertex2i(90, 70);
        glVertex2i(10, 10);

        // glVertex2i(0, 250);
        // glVertex2i(100, 150);

        /* glVertex3fv(vertices[0]);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]); */
    glEnd();

    glFlush();   // force execution of GL commands
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);                        // initialize toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // set display mode
    glutInitWindowSize(640, 480);                 // set window size
    glutInitWindowPosition(1, 1);                 // set window initial position
    glutCreateWindow("My First OpenGL Program");  // create a window

    glutDisplayFunc(display);                     // register redraw callback
    glutKeyboardFunc(keyEvent);                   // register keyboard handler

    init();                                       // additional initializations

    glutMainLoop();                               // perpetual event processing loop
    return 0;
}
