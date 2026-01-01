#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include "TextureManager.h"

void GameManager::init() {
    if (camera) delete camera;
    if (environment) delete environment;
    if (house) delete house;
    ObjectHandler::clear();

    TextureManager::init(TextureID::IRON, "./libTexture/iron022.bmp");

    oldTime = glutGet(GLUT_ELAPSED_TIME);
    camera = new Camera(initialCameraPos);
    environment = new Environment(skyColor);
    house = (new Object(Vec3(0.0f, 0.0f, 0.0f), false))
            ->addChildren(new Sphere(-2.5f, 2.5f, -4.0f, false, TextureID::IRON))
            ->addChildren(new Cube(1.0f, 2.5f, -6.0f, false, TextureID::IRON))
            ->addChildren(new Cube(4.5f, 2.5f, -5.0f, false, TextureID::IRON))
            ->addChildren(new Cube(0.0f, 0.0f, 0.0f, false, TextureID::IRON))
            ->addChildren(new Cube(1.0f, 0.0f, 1.0f, false, TextureID::IRON));
    house->setHidden(false);
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

Object *GameManager::getHouse() {
    return house;
}