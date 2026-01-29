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

void GameManager::loadLevelAssets() {
    //! Helper function
    auto addAsset = [&](std::string path, Vec3<GLfloat> pos) {
        Object* obj = AssetLoader::load(path, pos);
        if (obj) {
            obj->optimize();
            levelAssets.push_back(obj);
            house->addChildren(obj);
        }

        return obj;
    };

    addAsset("../assets/sofa.txt", Vec3<GLfloat>(9.85f, 0.2f, 2.0f));
    addAsset("../assets/sofa.txt", Vec3<GLfloat>(8.8f, 0.0f, -11.7f));
    addAsset("../assets/stove.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/fridge.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/bed.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/coffee.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(5.5f, 0.3f, 1.0f))->setRotation(-90, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/door.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/door2.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/door3.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/table.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/washing_machine.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/bathtub.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/wardrobe.txt", Vec3<GLfloat>(6.5, 0.27, -8.45))->setRotation(-90, Vec3<GLfloat>(0, 1, 0))->setScale(Vec3<GLfloat>(0.8, 0.8, 1));
    addAsset("../assets/bookshelf.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/bathroommirror.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/basin.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/faucet.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/dryer.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/toilet.txt", Vec3<GLfloat>(3.34, 0.3, -3))->setRotation(90, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(3.5f, 0.3f, 1.0f))->setRotation(90, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(4.5, 0.3, 2.33))->setRotation(180, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(4.5, 0.3, -0.467));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(5.8, 0.0, -12.3))->setRotation(45, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/chair.txt", Vec3<GLfloat>(5.8, 0.0, -11))->setRotation(154, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/bed2.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/wardrobe.txt", Vec3<GLfloat>(9.67, 0.3, -4.67))->setRotation(180, Vec3<GLfloat>(0, 1, 0))->setScale(Vec3<GLfloat>(0.767, 0.767, 0.767));
    addAsset("../assets/kitchensink.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/toilet.txt", Vec3<GLfloat>(8.267, 0.3, -4.67))->setRotation(-90, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/fence.txt", Vec3<GLfloat>(4.8, 0.0, -13.5));
    addAsset("../assets/fence.txt", Vec3<GLfloat>(10, 0.0, -11.5))->setRotation(90, Vec3<GLfloat>(0, 1, 0));
    addAsset("../assets/fence.txt", Vec3<GLfloat>(8, 0.0, -13.5));
    addAsset("../assets/poster1.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/poster2.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/teda.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    addAsset("../assets/forest.txt", Vec3<GLfloat>(7.0f, 0.2f, 7.0f));
    addAsset("../assets/roof.txt", Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    Object *portal = AssetLoader::load("../assets/nether_portal.txt");
    portal->optimize();
    LightConfig config;
    config.position = {2.0f, 1.5f, 0.5f};
    config.color = {0.765625f, 0.0703125f, 0.85546875f};
    config.constant = 1.0f;
    config.linear = 0.5f;
    config.quadratic = 0.3f;
    LightingManager::registerLight(config, portal);
}

void GameManager::unloadLevelAssets() {
    for (Object* obj : levelAssets) {
        if (obj) {
            delete obj;
        }
    }
    levelAssets.clear();
}

void GameManager::init(int viewportX, int viewportY) {
    GameManager::viewportX = viewportX;
    GameManager::viewportY = viewportY;
    init();
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
    unloadLevelAssets();
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
    // Set projection and viewport
    GLint viewportWidth = width / viewportX;
    GLint viewportHeight = height / viewportY;
    for (int x = 0; x < viewportX; x++) {
        GLint startX = x * viewportWidth;
        for (int y = 0; y < viewportY; y++) {
            GLint startY = y * viewportHeight;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(fov, (GLdouble) viewportWidth / viewportHeight, near, far);
            glMatrixMode(GL_MODELVIEW);
            glViewport(startX, startY, startX + viewportWidth, startY + viewportHeight);
        }
    }

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