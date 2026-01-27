#ifndef SRC_PORCH_H
#define SRC_PORCH_H
#include "BuildingBlocks.h"
#include "House.h"
#include "../Object.h"

class Porch : public Block {
public:
    static inline GLfloat frontWidth = 6.8182f;
    static inline GLfloat frontLength = 2.33675f;
    static inline GLfloat leftLength = 3.33528f;

    static inline GLfloat ceilingSpacing = 0.3;
    static inline GLfloat ceilingHeight = 0.25f;

    static inline GLfloat firstLayerHeight = 0.1f;
    static inline GLfloat secondLayerHeight = 0.17f;
    static inline GLfloat secondLayerSpacing = 0.1f;

    static inline GLfloat pillarHeight = House::height - firstLayerHeight - secondLayerHeight - ceilingHeight;
    static inline GLfloat pillarWidth = 0.2f;
    static inline GLfloat pillarInset = 0.05f;

    static inline Color3f floorColor = {.red = 1, .green = 1, .blue = 1};
    static inline MaterialID floorMaterial = MaterialID::MATTE;
    static inline Color3f ceilingColor = {.red = 0.19140625, .green = 0.19140625, .blue = 0.19140625};
    static inline MaterialID ceilingMaterial = MaterialID::MATTE;

    static inline GLfloat totalWidth = frontWidth + 2 * (ceilingSpacing + secondLayerSpacing);
    static inline GLfloat totalHeight = firstLayerHeight + secondLayerHeight + pillarHeight + ceilingHeight;
    static inline GLfloat totalLength = frontLength + leftLength + 2 * (secondLayerSpacing + ceilingSpacing);
    
    Porch(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material) { performScaling(); Porch::updateDimensions(); Porch::addAll(); }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
protected:
    void addAll() override;

    std::vector<GLfloat *> getScalableVars() override {
        return {
            &frontWidth,
            &frontLength,
            &leftLength,
            &ceilingHeight,
            &ceilingSpacing,
            &pillarHeight,
            &pillarWidth,
            &pillarInset,
            &secondLayerHeight,
            &secondLayerSpacing,
            &firstLayerHeight
        };
    }
    
    void updateDimensions() override {
        totalWidth = frontWidth + 2 * (ceilingSpacing + secondLayerSpacing);
        totalHeight = firstLayerHeight + secondLayerHeight + pillarHeight + ceilingHeight;
        totalLength = frontLength + leftLength + 2 * (secondLayerSpacing + ceilingSpacing);
    }
private:
    Object *addFloor();
    Object *addCeiling();
    Object *addPillars();
};

class FlowerPot : public Block {
public:
    Color3f flowerColor;

    static inline GLfloat potSize = 0.4f;
    static inline GLfloat potHeight = 0.4f;
    static inline GLfloat stemHeight = 0.5f;
    static inline GLfloat stemThick = 0.05f;

    static inline Color3f clayColor = {0.8f, 0.4f, 0.2f};
    static inline Color3f soilColor = {0.3f, 0.2f, 0.1f};
    static inline Color3f stemColor = {0.1f, 0.8f, 0.1f};

    static inline GLfloat totalWidth = 0.0f;
    static inline GLfloat totalHeight = 0.0f;
    static inline GLfloat totalLength = 0.0f;

    FlowerPot(Vec3<GLfloat> pos, GLfloat scale = 1.0f, Color3f flowerColor = {1.0f, 0.0f, 0.0f})
        : flowerColor(flowerColor), Block(pos, scale) { performScaling(); FlowerPot::updateDimensions(); FlowerPot::addAll(); }
protected:
    void addAll() override;

    std::vector<GLfloat *> getScalableVars() override {
        return {
            &potSize,
            &potHeight,
            &stemHeight,
            &stemThick
        };
    }

    void updateDimensions() override {
        totalWidth = potSize;
        totalHeight = potHeight + stemHeight;
        totalLength = potSize;
    }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
};

#endif //SRC_PORCH_H