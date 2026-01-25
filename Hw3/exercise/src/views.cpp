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

Object* housePtr = nullptr;
Object* roofPtr = nullptr;

void drawScene() {
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    glDepthMask(GL_FALSE);
    for (Object *o : trans) {
        if (!o->isHidden()) {
            o->draw();
        }
    }
    glDepthMask(GL_TRUE);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    int vw = w / 2;
    int vh = h / 2;

    glViewport(0, h/2, vw, vh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 30.0, -10.0, 25.0, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(12.0, 50.0, 8.0,
              12.0, 0.0, 8.0,
              0.0, 0.0, -1.0);

    if (roofPtr) roofPtr->setHidden(true);
    drawScene();


    // --- VIEW 2: Top-Down Floor Plan ---
    glViewport(w/2, h/2, vw, vh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 30.0, -10.0, 25.0, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(12.0, 50.0, 8.0,
              12.0, 0.0, 8.0,
              0.0, 0.0, -1.0);
    if (roofPtr) roofPtr->setHidden(false);
    drawScene();


    // --- VIEW 3: Front View ---
    glViewport(0, 0, vw, vh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(GameManager::fov, (GLfloat)vw/vh, GameManager::near, GameManager::far);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Vec3<GLfloat> camPos = GameManager::initialCameraPos;
    gluLookAt(camPos.x, camPos.y, camPos.z,
              8.0, 2.0, 5.0,
              0.0, 1.0, 0.0);
    if (roofPtr) roofPtr->setHidden(false);
    drawScene();


    // --- VIEW 4: Interior ---
    glViewport(w/2, 0, vw, vh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(GameManager::fov, (GLfloat)vw/vh, GameManager::near, GameManager::far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Position inside the Family Room / Dining Room
    gluLookAt(8.0, 1.7, 5.0,
              15.0, 1.5, 5.0,
              0.0, 1.0, 0.0);
    if (roofPtr) roofPtr->setHidden(false);
    drawScene();

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);

    WindowManager::init(display, idle);
    MaterialManager::init();
    TextureManager::init();
    GameManager::init();

    size_t maxChildren = 0;
    for (Object* obj : ObjectHandler::getObjects()) {
        if (obj->getChildren().size() > maxChildren) {
            maxChildren = obj->getChildren().size();
            housePtr = obj;
        }
    }

    if (housePtr && !housePtr->getChildren().empty()) {
        roofPtr = housePtr->getChildren().back();
    }

    glutMainLoop();
    return 0;
}