#include "GameManager.h"
#include "Object.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "AssetLoader.h"
#include "LightingManager.h"
#include "WindowManager.h"
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

void GameManager::loadLevelAssets() {
    house->addChildren(AssetLoader::addAsset(getAssetPath("sofa.txt"), Vec3<GLfloat>(9.85f, 0.2f, 2.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("sofa.txt"), Vec3<GLfloat>(8.8f, 0.0f, -11.7f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("stove.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("fridge.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("bed.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("coffee.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(5.5f, 0.3f, 1.0f))->setRotation(-90, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("door.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("door2.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("door3.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("table.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("washing_machine.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("bathtub.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("wardrobe.txt"), Vec3<GLfloat>(6.5, 0.27, -8.45))->setRotation(-90, Vec3<GLfloat>(0, 1, 0))->setScale(Vec3<GLfloat>(0.8, 0.8, 1)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("bookshelf.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("basin.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("faucet.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("dryer.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("toilet.txt"), Vec3<GLfloat>(3.34, 0.3, -3))->setRotation(90, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(3.5f, 0.3f, 1.0f))->setRotation(90, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(4.5, 0.3, 2.33))->setRotation(180, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(4.5, 0.3, -0.467)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(5.8, 0.0, -12.3))->setRotation(45, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("chair.txt"), Vec3<GLfloat>(5.8, 0.0, -11))->setRotation(154, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("bed2.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("wardrobe.txt"), Vec3<GLfloat>(9.67, 0.3, -4.67))->setRotation(180, Vec3<GLfloat>(0, 1, 0))->setScale(Vec3<GLfloat>(0.767, 0.767, 0.767)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("kitchensink.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("toilet.txt"), Vec3<GLfloat>(8.267, 0.3, -4.67))->setRotation(-90, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("fence.txt"), Vec3<GLfloat>(4.8, 0.0, -13.5)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("fence.txt"), Vec3<GLfloat>(10, 0.0, -11.5))->setRotation(90, Vec3<GLfloat>(0, 1, 0)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("fence.txt"), Vec3<GLfloat>(8, 0.0, -13.5)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("poster1.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("poster2.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    house->addChildren(AssetLoader::addAsset(getAssetPath("teda.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
    AssetLoader::addAsset(getAssetPath("forest.txt"), Vec3<GLfloat>(7.0f, 0.2f, 7.0f));
    house->addChildren(AssetLoader::addAsset(getAssetPath("poster3.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f)));
}

void GameManager::init() {
    oldTime = glutGet(GLUT_ELAPSED_TIME);
    
    camera = new Camera(initialCameraPos);
    
    environment = new Environment(skyColor);
    environment->spawn(); 
    environment->init();

    house = new House(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    loadLevelAssets(); 
}

void GameManager::reloadAssets() {
    AssetLoader::unloadLevelAssets();
    loadLevelAssets();
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
    //! Set projection and viewport
    if (WindowManager::getMode()) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, (GLdouble) width / height, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, width, height);
    }

    glEnable(GL_DEPTH_TEST);

    //! Initialize anything that has a per window context
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

House *GameManager::getHouse() {
    return house;
}

void GameManager::setRootPath(const std::string rootPath) {
    GameManager::rootPath = rootPath;
}

std::string GameManager::getRootPath() {
    return std::string(rootPath) + "/";
}

std::string GameManager::getAssetPath(std::string asset) {
    return getRootPath() + "assets/" + asset;
}

std::string GameManager::getTexturePath(std::string texture) {
    return getRootPath() + "textures/" + texture;
}