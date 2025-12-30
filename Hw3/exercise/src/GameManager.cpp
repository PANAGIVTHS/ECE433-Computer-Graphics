#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include <stdio.h>

void GameManager::init() {
    if (camera) delete camera;
    if (environment) delete environment;
    if (house) delete house;
    ObjectHandler::clear();

    oldTime = glutGet(GLUT_ELAPSED_TIME);
    camera = new Camera(initialCameraPos);
    environment = new Environment(skyColor);
    house = new House();
    environment->init();
    house->init();
}

void GameManager::updateDeltaTime() {
    GLint newTime = glutGet(GLUT_ELAPSED_TIME);
    GLint diffMs = newTime - oldTime;
    oldTime = newTime;

    dt = (GLdouble) diffMs / 1000.0f;
}

void GameManager::updatePositions() {
    for(Object *o : ObjectHandler::getObjects())
        o->update();

    InputManager::applyInputToCamera();
    camera->update();
}

void GameManager::onWindowUpdate(GLint width, GLint height, bool newContext) {
    // Set projection and viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLdouble) width / height, near, far);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);

    // Set other OpenGL settings
    glEnable(GL_DEPTH_TEST);

    // Initialize anything that has a per window context
    if (newContext) {
        if(environment) environment->init();
        if(house) house->init();
    }
}

void GameManager::cleanUp() {
    delete camera;
    delete environment;
    delete house;
    ObjectHandler::clear();
}

Camera *GameManager::getCamera() {
    return camera;
}

Environment *GameManager::getEnvironment() {
    return environment;
}

House *GameManager::getHouse() {
    return house;
}