#include <GL/glut.h>
#include <stdlib.h>

/* ---------- Global toggles ---------- */
int smoothShading = 1;
int cullingMode = 0;   // 0 = none, 1 = back, 2 = front

/* ---------- Initialization ---------- */
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* Material (red) */
    GLfloat mat_amb[]  = {0.3, 0.0, 0.0, 1.0};
    GLfloat mat_diff[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shine[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);

    /* Light */
    GLfloat light_pos[] = {3.0, 3.0, 3.0, 1.0};
    GLfloat light_col[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat ambient[]   = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_col);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_col);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
}

/* ---------- Display ---------- */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Camera OUTSIDE the sphere */
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    /* Shading mode */
    glShadeModel(smoothShading ? GL_SMOOTH : GL_FLAT);

    /* Culling mode */
    if (cullingMode == 0)
        glDisable(GL_CULL_FACE);
    else {
        glEnable(GL_CULL_FACE);
        glCullFace(cullingMode == 1 ? GL_BACK : GL_FRONT);
    }

    glutSolidSphere(1.0, 10, 20);

    glutSwapBuffers();
}

/* ---------- Reshape ---------- */
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
}

/* ---------- Keyboard ---------- */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 's':
            smoothShading = !smoothShading;
            break;

        case 'c':
            cullingMode = (cullingMode + 1) % 3;
            break;

        case 'q': 
            exit(0);

        case 27: // ESC
            exit(0);
    }
    glutPostRedisplay();
}

/* ---------- Main ---------- */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Red Sphere – Illumination & Culling");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
