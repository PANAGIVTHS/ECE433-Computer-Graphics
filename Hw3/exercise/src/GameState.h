#ifndef GAMESTATE_H
#define GAMESTATE_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Object.h"
#include "Camera.h"
#include "Environment.h"
#include "House.h"

class GameState {
private:
    inline static Camera *camera = nullptr;
    inline static Environment *environment = nullptr;
    inline static House *house = nullptr;
public:
    static constexpr GLfloat speed = 0.2f;
    static constexpr GLfloat sensitivity = 0.04f;
    static constexpr GLdouble fov = 90.0f;
    static constexpr GLdouble near = 0.1;
    static constexpr GLdouble far = 100;
    inline static const Vec3<GLfloat> initialCameraPos = Vec3(0.0f, 0.0f, 0.0f);
    inline static const Color3f skyColor = {.red = 0.3828125f, .green = 0.75390625f, .blue = 0.89453125f};

    inline static void init() {
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

    inline static void onWindowUpdate(GLint width, GLint height, bool newContext) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, (GLdouble) width / height, near, far);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, width, height);

        // Initialize anything that is per window
        if (newContext) {
            if(environment) environment->init();
            if(house) house->init();
        }
    }

    inline static void cleanUp() {
        delete camera;
        delete environment;
        delete house;
        ObjectHandler::clear();
    }

    inline static Camera *getCamera() {
        return camera;
    }

    inline static Environment *getEnvironment() {
        return environment;
    }

    inline static House *getHouse() {
        return house;
    }
};

#endif
