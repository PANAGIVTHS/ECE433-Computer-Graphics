#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <algorithm>
#include <iostream>

#include "Object.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "LightingManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

void init(int argc, char *argv[]);
void display();
void setupPerspective(int x, int y, int w, int h, 
                      Vec3<GLfloat> eye, Vec3<GLfloat> center, Vec3<GLfloat> up);
void setupOrtho(int x, int y, int w, int h, GLfloat zoom, bool topDown);
void displayScene(Vec3<GLfloat> camPos);
void keyboardDown(unsigned char key, int x, int y);
void specialKeyboardDown(int key, int x, int y);
int pause = 0;

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'p' - Toggle playback on top-right view.\n");
	printf("'ESC' - Quit the application.\n");
	printf("'F11' - Toggle fullscreen.\n");

    glutMainLoop();
    return 0;
}

void init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    WindowManager::init(display, nullptr, 0);
    MaterialManager::init();
    TextureManager::init();
    GameManager::init();
    glutKeyboardFunc(keyboardDown);
    glutSpecialFunc(specialKeyboardDown);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    int W = WindowManager::getWidth();
    int H = WindowManager::getHeight();
    int halfW = W / 2;
    int halfH = H / 2;
    House *house = GameManager::getHouse();

    // ---------------------------------------------------------
    // Botton left floor plan
    // ---------------------------------------------------------
    house->getRoof()->setHidden(true); 
    house->getGarageCeiling()->setHidden(true);

    setupOrtho(0, 0, halfW, halfH, 8.0f, true);
    displayScene(Vec3(10.0f, 50.0f, -2.5f));

    // ---------------------------------------------------------
    // Bottom right floor plan with roof
    // ---------------------------------------------------------
    house->getRoof()->setHidden(false); 
    house->getGarageCeiling()->setHidden(false);

    setupOrtho(halfW, 0, halfW, halfH, 8.0f, true);
    displayScene(Vec3(10.0f, 50.0f, -2.5f));

    // ---------------------------------------------------------
    // Top left front view perspective
    // ---------------------------------------------------------
    Vec3<GLfloat> frontEye(14.570936, 1.700000, 11.138914);
    Vec3<GLfloat> frontCenter = frontEye + Vec3<GLfloat>(-0.472914, -0.022165, -0.880830);
    
    setupPerspective(0, halfH, halfW, halfH, 
                     frontEye, frontCenter, Vec3(0.0f, 1.0f, 0.0f));
    displayScene(frontEye);

    // ---------------------------------------------------------
    // Top right interior cycling
    // ---------------------------------------------------------
    int time = glutGet(GLUT_ELAPSED_TIME);
    int period = 4000; // 4 seconds per room
    int phase = pause ? phase : (time / period) % 4;

    switch (phase) {
        case 0: // Family Room / Entrance
            frontEye = Vec3<GLfloat>(8.135541, 1.700000, -0.567432);
            frontCenter = frontEye + Vec3<GLfloat>(-0.893911, -0.131046, 0.428660);
            break;
        case 1: // Dining Room / Kitchen area
            frontEye = Vec3<GLfloat>(6.072436, 1.700000, 3.437286);
            frontCenter = frontEye + Vec3<GLfloat>(0.750779, -0.120657, -0.649441);
            break;
        case 2: // Bedroom / Hallway
            frontEye = Vec3<GLfloat>(9.445059, 1.700000, -9.201321);
            frontCenter = frontEye + Vec3<GLfloat>(-0.413258, -0.110250, 0.903915);
            break;
        case 3: // Garage
            frontEye = Vec3<GLfloat>(14.818394, 1.700000, 4.067469);
            frontCenter = frontEye + Vec3<GLfloat>(-0.472939, 0.014660, -0.880973);
            break;
    }
    
    setupPerspective(halfW, halfH, halfW, halfH, 
                     frontEye, frontCenter, Vec3(0.0f, 1.0f, 0.0f));
    displayScene(frontEye);

    glutSwapBuffers();
    glutPostRedisplay();
}

void setupPerspective(int x, int y, int w, int h, 
                      Vec3<GLfloat> eye, Vec3<GLfloat> center, Vec3<GLfloat> up) {
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, 
              center.x, center.y, center.z, 
              up.x, up.y, up.z);
}

void setupOrtho(int x, int y, int w, int h, GLfloat zoom, bool topDown) {
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    if (w <= h)
        glOrtho(-zoom, zoom, -zoom/aspect, zoom/aspect, -100.0, 100.0);
    else
        glOrtho(-zoom*aspect, zoom*aspect, -zoom, zoom, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (topDown) {
        gluLookAt(10.0, 50.0, -2.5,
                  10.0, 0.0, -2.5,
                  0.0, 0.0, -1.0);
    }
}

void displayScene(Vec3<GLfloat> camPos) {
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    
    // //! Sort transparent objects (Farthest -> Nearest)
    std::sort(trans.begin(), trans.end(), [camPos](Object* a, Object* b) {
        return (a->getWorldPosition() - camPos).magSq() > (b->getWorldPosition() - camPos).magSq();
    });

    glDepthMask(GL_FALSE);
    for (Object *o : trans) {
        o->draw();
    }
    glDepthMask(GL_TRUE);
}

void keyboardDown(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            GameManager::cleanUp();
            exit(0);
        case 'p': 
        case 'P':
            pause = !pause;
            break;
    }
}

void specialKeyboardDown(int key, int x, int y) {
    if (key != GLUT_KEY_F11)
        return;
    
    WindowManager::switchMode();
}