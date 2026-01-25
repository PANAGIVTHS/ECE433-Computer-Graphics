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

    addAsset("../assets/sofa.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/sofa2.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/stove.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/fridge.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/bed.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/coffee.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/chair.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/door.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/door2.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/door3.txt", Vec3(0.0f, 0.0f, 0.0f));
    addAsset("../assets/table.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/washing_machine.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/bathtub.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/wardrobe.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/bookshelf.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/bathroommirror.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/basin.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/faucet.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/dryer.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/toilet.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/chair2.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/chair3.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/chair4.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/bed2.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/wardrobe2.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/kitchensink.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/toilet2.txt", Vec3(0.0f, 0.0f, -0.0f));
    addAsset("../assets/fence.txt", Vec3(0.0f, 0.0f, -0.0f));

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
    Model *car = new Model("../assets/Car.obj", Vec3(13.0f, 0.2f, -3.0f), false, {1.0f, 1.0f, 1.0f}, TextureID::NONE, MaterialID::MATTE);
    car->optimize();
    house->addChildren(car);

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