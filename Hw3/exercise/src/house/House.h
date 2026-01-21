#ifndef SRC_HOUSE_H
#define SRC_HOUSE_H
#include "../Object.h"

class House : public Object {
    static std::vector<GLfloat *> getScalableVars() {
        return {
            &height, &interiorWallThickness, &exteriorWallThickness, &ridgeThickness
        };
    }

    static void performScaling() {
        if (scale == 1.0f) return;

        std::vector<GLfloat*> vars = getScalableVars();
        for (GLfloat* ptr : vars) {
            if (ptr) *ptr *= scale;
        }
    }

    void setup();
public:
    static inline GLfloat scale = 1.3f;
    static inline GLfloat height = 3.5f;
    static inline GLfloat interiorWallThickness = .10f;
    static inline GLfloat exteriorWallThickness = .25f;
    static inline GLfloat ridgeThickness = 0.1f;

    static inline Color3f lightColor = {.red = 1.0f, .green = 1.0f, .blue = 1.0f};
    static inline Color3f darkColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static inline Color3f floorColor = {0.55f, 0.57f, 0.58f};

    House(Vec3<GLfloat> pos, bool gravity = DEFAULT_GRAVITY, Color3f color = {.red = 1, .green = 1, .blue = 1}, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material) { performScaling(); setup(); }
};

#endif //SRC_HOUSE_H