#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include "TextureManager.h"

//TODO idk where to put this (dont say the joke haha we get it)
void loadGameTextures() {
    TextureManager::init(TextureID::IRON, "../Texture_Images/LaGioconda.bmp");
    TextureManager::init(TextureID::GRASS, "../Texture_Images/grass.raw", 256, 256);
    TextureManager::init(TextureID::WINDOW, "../Texture_Images/ss0052.bmp");
}

void GameManager::init() {
    if (camera) delete camera;
    if (environment) delete environment;
    if (house) delete house;
    ObjectHandler::clear();

    loadGameTextures();
    oldTime = glutGet(GLUT_ELAPSED_TIME);
    camera = new Camera(initialCameraPos);
    environment = new Environment(skyColor);
    house = (new Object(Vec3(0.0f, 0.0f, 0.0f), false))
            ->addChildren(new Sphere(-2.5f, 2.5f, -4.0f, false, TextureID::WINDOW))
            ->addChildren(new Cuboid(1.0f, 2.5f, -6.0f, 0.5f, 4.0f, 0.5f, false, TextureID::IRON))
            ->addChildren(new Cuboid(4.5f, 2.5f, -5.0f, 2.0f, 2.0f, 2.0f, false, TextureID::GRASS, TextureConfig(TextureMode::REPEAT_CUSTOM, 6.7f, 3.0f)))
            ->addChildren(new Cuboid(1.0f, 0.0f, 0.0f,  4.0f, 0.5f, 0.2f, false, TextureID::IRON))
            ->addChildren(new Cuboid(1.0f, 0.0f, 1.0f,  0.5f, 0.5f, 5.0f, false, TextureID::IRON));
            //->addChildren(new Cube(0.0f, 3.0f, 0.0f, false, TextureID::IRON));
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

        //TODO should this be done like this? Simplest way for sure but idk
        //! Delete old unreachable textures
        TextureManager::clear();
        loadGameTextures();
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