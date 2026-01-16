#ifndef SRC_GARAGE_H
#define SRC_GARAGE_H
#include "../Object.h"

class Garage : public Object {
public:
    Garage(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { addAll(); }
private:
    GLfloat wallThickness = 0.25f;
    GLfloat width = 7.3152f;
    GLfloat length = 3.8608f;
    GLfloat height = 4.0f;
    GLfloat ridgeThickness = 0.1f;
    GLfloat doorWidth = 3.0f;
    GLfloat doorHeight = 2.4f;
    GLfloat sidePanelWidth = (length - doorWidth) / 2.0f;
    GLfloat headerHeight = height - doorHeight;

    void addAll();
    Object *addRightSide();
    Object *addFrontSide();
    Object *addBackSide();
    Object *addCeiling();
    Object *addDoor(Object *front);
    void drawInternal() override {}
};

#endif //SRC_GARAGE_H