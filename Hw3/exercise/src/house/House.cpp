#include "House.h"
#include "Garage.h"
#include "Porch.h"

class BorderCubes : public Object {
public:
    static constexpr float CUBE_SIZE = 0.1f;

    BorderCubes(Vec3<GLfloat> startPos, float width, float length, float height,
                bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR)
        : Object(startPos, gravity, color, TextureID::NONE, MaterialID::MATTE) {

        createCorners(width, height, length);
    }

private:
    void createCorners(float w, float h, float l) {
        // Calculate the 8 corners relative to the starting position (0,0,0 local)
        // We use {0} and {dimension} to define the min and max bounds for each axis
        std::vector<Vec3<float>> positions = {
            Vec3(0.0f, 0.0f, 0.0f), // Bottom-Left-Back
            Vec3(w,    0.0f, 0.0f), // Bottom-Right-Back
            Vec3(0.0f, h,    0.0f), // Top-Left-Back
            Vec3(0.0f, 0.0f, l),    // Bottom-Left-Front
            Vec3(w,    h,    0.0f), // Top-Right-Back
            Vec3(w,    0.0f, l),    // Bottom-Right-Front
            Vec3(0.0f, h,    l),    // Top-Left-Front
            Vec3(w,    h,    l)     // Top-Right-Front
        };

        // Create a Cube at each corner position
        for (const auto& pos : positions) {
            Color3f color = {.red = 1, .green = 1, .blue = 1};
            if (pos.x == 0 && pos.y == 0 && pos.z == 0) color = {.red = 1, .green = 0, .blue = 0};
            Object* corner = new GlowingCube(
                pos,
                Vec3(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE),
                false, // Markers generally shouldn't fall
                color,
                TextureID::NONE,
                MaterialID::MATTE
            );
            addChildren(corner);
        }
    }

    // No internal drawing needed, just the children
    void drawInternal() override {}
};

void House::setup() {
    Porch *porch = new Porch(0, 0, 0);
    addChildren(porch);

    BorderCubes *cubes = new BorderCubes(porch->getPosition(), Porch::totalWidth, Porch::totalLength, Porch::totalHeight);
    addChildren(cubes);

    Vec3<GLfloat> garagePos = porch->getPosition() + Vec3<GLfloat>(Porch::totalWidth, 0, 0);
    Garage *garage = new Garage(garagePos);
    addChildren(garage);

    BorderCubes *cubes1 = new BorderCubes(garagePos, Garage::totalWidth, Garage::totalLength, Garage::totalHeight);
    addChildren(cubes1);
}
