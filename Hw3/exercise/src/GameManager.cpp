#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "AssetLoader.h"
#include "LightingManager.h"
<<<<<<< HEAD
#include <iostream>
=======
>>>>>>> from-working

//fps related

void GameManager::updateFPS() {
    frameCount++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    
    if (currentTime - lastFpsTime > 1000) {
        fps = frameCount * 1000.0f / (currentTime - lastFpsTime);
        lastFpsTime = currentTime;
        frameCount = 0;
    }
}

float GameManager::getFPS() {
    return fps;
}

void GameManager::init() {
    if (camera) delete camera;
    if (environment) delete environment;
    ObjectHandler::clear();

    oldTime = glutGet(GLUT_ELAPSED_TIME);
    camera = new Camera(initialCameraPos);
    environment = new Environment(skyColor);
    environment->spawn();
    
    // Object *house = AssetLoader::load("../assets/house.txt", Vec3(0.0f, 3.0f, -10.0f));
    // std::cout << house->toString() << "\n";

    Garage *garage = new Garage(0.0f, 2.0f, -10.0f);
    std::cout << garage->toString() << "\n";

    AssetLoader::load("../assets/oaktree.txt", Vec3(7.0f, 0.2f, 7.0f));
    AssetLoader::load("../assets/pinetree.txt", Vec3(1.0f, 0.2f, 2.0f));
    AssetLoader::load("../assets/pinetree.txt", Vec3(10.0f, 0.0f, 10.0f));

    environment->init();
}

void GameManager::updateDeltaTime() {
    GLint newTime = glutGet(GLUT_ELAPSED_TIME);
    GLint diffMs = newTime - oldTime;
    oldTime = newTime;

    dt = (GLdouble) diffMs / 1000.0f;
}

void GameManager::runGameLoop() {
    updateDeltaTime();
    updateFPS();

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
        //! Delete old unreachable textures
        ObjectHandler::resetDisplayListAll(false);
        TextureManager::clear();

        LightingManager::init();
<<<<<<< HEAD
        TextureManager::init();
        if(environment) environment->init();
=======
        ObjectHandler::invalidateListAll();
        TextureManager::init();
>>>>>>> from-working
    }
}

void GameManager::cleanUp() {
    delete camera;
    delete environment;
    ObjectHandler::clear();
}

Camera *GameManager::getCamera() {
    return camera;
}

Environment *GameManager::getEnvironment() {
    return environment;
}