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
// flashlight
int flashlightID = -1;
LightConfig flashlightConfig;

void drawGluSpline() {
    // 1. Initialize the NURBS Renderer (Only once)
    static GLUnurbsObj* theNurb = nullptr;
    if (!theNurb) {
        theNurb = gluNewNurbsRenderer();
        
        // Settings:
        // GLU_SAMPLING_TOLERANCE: Smaller = smoother curve (more tessellation)
        gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
        // GLU_DISPLAY_MODE: GLU_OUTLINE_POLYGON ensures it draws lines, not filled shapes
        gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
    }

    // 2. Define Control Points (Same spiral loop as before)
    // 11 points * 3 coordinates (x, y, z)
    static GLfloat ctlpoints[11][3] = {
        {0, 10, 0},    {5, 12, 5},    {10, 8, 0}, 
        {5, 4, -5},    {0, 2, 0},     {-5, 4, 5}, 
        {-10, 8, 0},   {-5, 12, -5},  {0, 10, 0}, 
        {5, 12, 5},    {10, 8, 0}
    };

    // 3. Define Knot Vector
    // Rule: numKnots = numControlPoints + Order
    // We have 11 points and Order 4 (Cubic). So we need 15 knots.
    // "Clamped" knot vector (0000...1111) makes the curve touch the start/end points.
    static GLfloat knots[15] = {
        0.0, 0.0, 0.0, 0.0,   // Start clamp (Order 4)
        1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, // Internal knots
        8.0, 8.0, 8.0, 8.0    // End clamp (Order 4)
    };

    // 4. Draw Style Setup
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT); // Save current OpenGL state
    glDisable(GL_LIGHTING);       // Make it solid color
    glDisable(GL_TEXTURE_2D);     // No textures
    glColor3f(0.0f, 1.0f, 1.0f);  // Cyan Color
    glLineWidth(4.0f);            // Thick line

    // 5. Render the NURBS Curve
    gluBeginCurve(theNurb);
    gluNurbsCurve(
        theNurb, 
        15,                 // Knot count
        knots,              // Knot array
        3,                  // Stride (distance between points in array: 3 floats)
        &ctlpoints[0][0],   // Control points array
        4,                  // Order (4 = Cubic)
        GL_MAP1_VERTEX_3    // Type (3D Vertex)
    );
    gluEndCurve(theNurb);

    // 6. Restore State
    glPopAttrib(); // Restore lighting/textures/linewidth to whatever they were before
}

// Update in src/visiting3Dhouse.cpp
void drawGluSurface() {
    // Static variable to store the Display List ID
    static GLuint nurbDisplayList = 0;

    if (nurbDisplayList == 0) {
        // --- ONE-TIME INITIALIZATION ---
        
        nurbDisplayList = glGenLists(1);
        
        GLUnurbsObj* theNurb = gluNewNurbsRenderer();
        gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL); 
        gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0); 

        // 3. Define Data (Control Points & Knots)
        // SHIFTED Y-VALUES: Added +18.0 to every Y so the lowest point (-8) becomes 10.
        static GLfloat ctlpoints[4][4][3] = {
            // Row 0 (Front) - Old Ys: -2, 5, -5, -2
            {{-10., 16., -10.}, {-5., 23., -10.}, {5., 13., -10.}, {10., 16., -10.}},
            // Row 1 - Old Ys: 0, 8, -8, 0
            {{-10., 18., -5.}, {-5., 26., -5.}, {5., 10., -5.}, {10., 18., -5.}},
            // Row 2 - Old Ys: 0, 8, -8, 0
            {{-10., 18., 5.}, {-5., 26., 5.}, {5., 10., 5.}, {10., 18., 5.}},
            // Row 3 (Back) - Old Ys: -2, 5, -5, -2
            {{-10., 16., 10.}, {-5., 23., 10.}, {5., 13., 10.}, {10., 16., 10.}}
        };

        static GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

        glNewList(nurbDisplayList, GL_COMPILE);
        
            glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
            glEnable(GL_LIGHTING);
            glEnable(GL_AUTO_NORMAL);
            glEnable(GL_NORMALIZE);
            
            MaterialManager::bind(MaterialID::MATTE); 

            gluBeginSurface(theNurb);
            gluNurbsSurface(
                theNurb,
                8, knots,
                8, knots,
                4 * 3,
                3,
                &ctlpoints[0][0][0],
                4, 4,
                GL_MAP2_VERTEX_3
            );
            gluEndSurface(theNurb);

            glPopAttrib();

        glEndList();

        gluDeleteNurbsRenderer(theNurb);
    }

    if (nurbDisplayList != 0) {
        glCallList(nurbDisplayList);
    }
}

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
    //drawGluSurface();
    
    for (Object *o : ObjectHandler::getObjects()) {
        if (!o->hasTransparency()) {
            o->draw();
        }
    }

    // 2. Transparent Pass
    std::vector<Object*>& trans = ObjectHandler::getTransObjects();
    Vec3<float> camPos = GameManager::getCamera()->getPosition();
    
    // Sort transparent objects (Farthest -> Nearest)
    std::sort(trans.begin(), trans.end(), [camPos](Object* a, Object* b) {
        return (a->getWorldPosition() - camPos).magSq() > (b->getWorldPosition() - camPos).magSq();
    });

    glDepthMask(GL_FALSE); // Read-only depth buffer
    for (Object *o : trans) {
        o->draw();
    }
    glDepthMask(GL_TRUE);

    //fps related
    WindowManager::drawFPS(GameManager::getFPS());

    glutSwapBuffers();
}

void idle() {
    GameManager::runGameLoop();
    glutPostRedisplay();
}