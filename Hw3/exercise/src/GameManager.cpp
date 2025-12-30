#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"

void GameManager::init() {
    if (camera) delete camera;
    if (environment) delete environment;
    if (house) delete house;
    ObjectHandler::clear();

    camera = new Camera(initialCameraPos);
    environment = new Environment(skyColor);
    house = new House();
    environment->init();
    house->init();
}

void GameManager::updatePositions() {
    for(Object *o : ObjectHandler::getObjects()) {
        if (o->hasGravity())
            o->getVelocity().y -= gravity;
        o->applyVelocity();
    }

    InputManager::applyInputToCamera();
    if (camera->hasGravity())
        camera->getVelocity().y -= gravity;
    camera->applyVelocity();
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