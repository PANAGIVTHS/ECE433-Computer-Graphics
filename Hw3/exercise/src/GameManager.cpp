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

    House *house = new House(0.0f, 0.0f, -10.0f);
    // std::cout << house->toString() << "\n";


    auto helixFunc = [](float t) -> Vec3<float> {
        return Vec3<float>(
            2.0f * cos(t), 
            t * 0.5f, 
            2.0f * sin(t)
        );
    };

    auto doubleTwist = [](float t) -> Vec3<float> {
        return {
            2.0f * cos(t),
            0.3f * t + 0.5f * sin(5.0f * t),
            2.0f * sin(2.0f * t)
        };
    };

    auto trefoil = [](float t) -> Vec3<float> {
        return {
            sin(t) + 2.0f * sin(2.0f * t),
            cos(t) - 2.0f * cos(2.0f * t),
            -sin(3.0f * t)
        };
    };


    // Create helix with 100 control points for smoothness
    (new NurbsCurve(trefoil, 0.0f, 6.0f * 3.14159f, 100, Vec3<float>(-5, 4, 0), 4, {1, 4, 0}))->optimize();

    auto rippleFunc = [](float u, float v) -> Vec3<float> {
        float r = sqrt(u*u + v*v);
        float y = (r == 0) ? 1.0f : sin(r) / r; // Sinc function
        return Vec3<float>(u, y * 3.0f, v);
    };

    // Create surface 20x20 grid
    (new NurbsSurface(rippleFunc, 
                     -5.0f, 5.0f, 20, // U range
                     -5.0f, 5.0f, 20, // V range
                     Vec3<float>(5, 5, 0), 
                     4, {0, 0.8f, 1.0f},
                     TextureID::NONE, MaterialID::MATTE))->optimize();

    AssetLoader::load("../assets/oaktree.txt", Vec3(7.0f, 0.2f, 7.0f))->optimize();
    AssetLoader::load("../assets/pinetree.txt", Vec3(1.0f, 0.2f, 2.0f))->optimize();
    AssetLoader::load("../assets/pinetree.txt", Vec3(10.0f, 0.0f, 10.0f))->optimize();

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