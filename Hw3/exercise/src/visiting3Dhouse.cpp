#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "Object.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "LightingManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <algorithm>
#include <vector>
#include "Spline.h"
// flashlight
int flashlightID = -1;
LightConfig flashlightConfig;

void init(int argc, char *argv[]);
void display();
void idle();

int main(int argc, char *argv[]) {
    init(argc, argv);

	printf("Keyboard commands:\n");
	printf("'w' - Move forward.\n");
	printf("'a' - Move left.\n");
	printf("'s' - Move backward.\n");
	printf("'d' - Move right.\n");
	printf("'SPACE' - Move up.\n");
	printf("'z' - Move down.\n");
#ifndef MOUSE_ROTATION
	printf("'i' - Look up.\n");
	printf("'j' - Look left.\n");
	printf("'k' - Look down.\n");
	printf("'l' - Look right.\n");
#endif
	printf("'ESC' - Quit the application.\n");
	printf("'F11' - Toggle fullscreen.\n");
#ifdef MOUSE_ROTATION
	printf("\n");
    printf("Move the mouse to look around!\n");
#endif

    glutMainLoop();
    return 0;
}

void init(int argc, char *argv[]) {
    glutInit(&argc, argv);
    WindowManager::init(display, idle);
    LightingManager::init();
    MaterialManager::init();
    TextureManager::init();
    GameManager::init();

    // flashlight
    flashlightConfig.position = Vec3<float>(0.0f, 0.0f, 0.0f); // At the Eye (Origin)
    flashlightConfig.color    = Vec3<float>(1.0f, 1.0f, 0.9f); // Slightly warm white
    
    // Spotlight settings (The cone of light)
    flashlightConfig.spotCutoff = 20.0f;           // 20 degree beam width
    flashlightConfig.spotExponent = 15.0f;         // High focus (brighter in center)
    flashlightConfig.spotDirection = Vec3<float>(0.0f, 0.0f, -1.0f); // Points straight forward
    
    // Attenuation (Makes it fade with distance so it looks realistic)
    flashlightConfig.constant = 1.0f;
    flashlightConfig.linear = 0.04f;
    flashlightConfig.quadratic = 0.0f;

    flashlightID = LightingManager::createLight(flashlightConfig);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // flashlight
    if (flashlightID != -1) {
        LightingManager::updateLight(flashlightID, flashlightConfig);
    }
    GameManager::getCamera()->set();

    LightingManager::updateAllLights();
    
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    Vec3<float> camPos = GameManager::getCamera()->getPosition();
    
    //! Sort transparent objects (Farthest -> Nearest)
    std::sort(trans.begin(), trans.end(), [camPos](Object* a, Object* b) {
        return (a->getWorldPosition() - camPos).magSq() > (b->getWorldPosition() - camPos).magSq();
    });

    glDepthMask(GL_FALSE);
    for (Object *o : trans) {
        o->draw();
    }
    glDepthMask(GL_TRUE);

    //fps related
    WindowManager::drawFPS(GameManager::getFPS());

    glutSwapBuffers();
}

void idle() {
    #if defined(MODEL)
    GameManager::reloadAssets();
    #endif
    GameManager::runGameLoop();
    glutPostRedisplay();
}