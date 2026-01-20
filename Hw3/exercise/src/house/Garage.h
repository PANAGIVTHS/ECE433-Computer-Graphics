#ifndef SRC_GARAGE_H
#define SRC_GARAGE_H
#include "../Object.h"

class Garage : public Object {
    static inline GLfloat floorHeight = 0.05f;
    static inline GLfloat wallThickness = 0.25f;

    static inline GLfloat width = 7.3152f;
    static inline GLfloat length = 1.2 * 3.8608f;
    static inline GLfloat height = 4.0f;

    static inline GLfloat ridgeThickness = 0.1f;
    static inline GLfloat doorWidth = 3.2f;
    static inline GLfloat doorHeight = 2.8f;
    static inline GLfloat sidePanelWidth = (length - doorWidth) / 2.0f;
    static inline GLfloat headerHeight = height - doorHeight;

    static inline Color3f wallColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static inline Color3f concreteColor = {0.55f, 0.57f, 0.58f};

    void addAll();
    Object *addFloor();
    Object *addRightSide();
    Object *addFrontSide();
    Object *addBackSide();
    Object *addCeiling();
    Object *addDoor(Object *front);
    void drawInternal() override {}
public:
    Garage(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = wallColor, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { addAll(); }
};

#endif //SRC_GARAGE_H