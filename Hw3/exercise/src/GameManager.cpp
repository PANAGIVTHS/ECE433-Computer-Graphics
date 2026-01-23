#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "AssetLoader.h"
#include "LightingManager.h"
#include "Model.h"
#include "Spline.h"
#include <iostream>

#include "house/House.h"

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

    House *house = new House(Vec3<GLfloat>(0.0f, 0.0f, -10.0f));
    AssetLoader::load("../assets/forest.txt", Vec3(7.0f, 0.2f, 7.0f))->optimize();
    AssetLoader::load("../assets/sofa.txt", Vec3(7.0f, 0.2f, 7.0f))->optimize();
    AssetLoader::load("../assets/stove.txt", Vec3(2.0f, 0.2f, 2.0f))->optimize();
    AssetLoader::load("../assets/fridge.txt", Vec3(1.0f, 0.2f, 2.5f))->optimize();
    AssetLoader::load("../assets/bed.txt", Vec3(10.0f, 0.2f, 2.5f))->optimize();
    AssetLoader::load("../assets/coffee.txt", Vec3(15.0f, 0.2f, 2.5f))->optimize();
    AssetLoader::load("../assets/chair.txt", Vec3(12.0f, 0.2f, 2.5f))->optimize();
    //Object *car = new Model("../assets/car.obj", Vec3(12.0f, 3.2f, 2.5f), false, {1.0f, 1.0f, 1.0f}, TextureID::NONE, MaterialID::MATTE);
    // car->setScale(0.1f, 0.1f, 0.1f);
    // car->optimize();

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
        TextureManager::clear();

        LightingManager::init();
        ObjectHandler::invalidateDisplayListAll();
        TextureManager::init();

        if(environment) environment->init();
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