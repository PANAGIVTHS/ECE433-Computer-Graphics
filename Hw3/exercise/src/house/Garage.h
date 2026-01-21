#ifndef SRC_GARAGE_H
#define SRC_GARAGE_H
#include "../Object.h"

class Garage : public Object {
public:
    static constexpr GLfloat floorHeight = 0.05f;
    static constexpr GLfloat wallThickness = 0.25f;

    static constexpr GLfloat width = 7.3152f;
    static constexpr GLfloat length = 1.2 * 3.8608f;
    static constexpr GLfloat height = 4.0f;

    static constexpr GLfloat ridgeThickness = 0.1f;
    static constexpr GLfloat doorWidth = 3.2f;
    static constexpr GLfloat doorHeight = 2.8f;
    static constexpr GLfloat sidePanelWidth = (length - doorWidth) / 2.0f;
    static constexpr GLfloat headerHeight = height - doorHeight;

    static constexpr Color3f wallColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static constexpr Color3f concreteColor = {0.55f, 0.57f, 0.58f};

    static constexpr MaterialID wallMaterial = MaterialID::MATTE;

private:
    void addAll();
    Object *addFloor();
    Object *addRightSide();
    Object *addFrontSide();
    Object *addBackSide();
    Object *addCeiling();
    Object *addDoor(Object *front);
    void drawInternal() override {}
public:
    Garage(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = wallColor, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { addAll(); }
    Garage(float x, float y, float z, bool gravity = DEFAULT_GRAVITY, Color3f color = wallColor, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(x, y, z, gravity, color, texture, material) { addAll(); }
};

#endif //SRC_GARAGE_H