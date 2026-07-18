#ifndef SRC_BUILDINGBLOCKS_H
#define SRC_BUILDINGBLOCKS_H
#include "../Object.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
class Block : public Object {
protected:
    GLfloat scale;

    virtual std::vector<GLfloat*> getScalableVars() = 0;
    virtual void updateDimensions() = 0;
    virtual void addAll() = 0;

    void performScaling() {
        if (scale == 1.0f) return;

        std::vector<GLfloat*> vars = getScalableVars();
        for (GLfloat* ptr : vars) {
            if (ptr) *ptr *= scale;
        }
    }
public:
    virtual Vec3<GLfloat> getDimensions() = 0;

    Block(Vec3<GLfloat> pos, GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), scale(scale) { }
};

class AnchoredCuboid : public Cuboid {
public:
    AnchoredCuboid(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL, TextureConfig config = TextureConfig(), int subdivisions = 1)
        : Cuboid(pos, dim, gravity, color, texture, material, config, subdivisions) { }

    Vec3<GLfloat> getPosition() const {
        return transform.position + dim / 2.0f;
    }
};

class AnchoredCube : public Cube {
public:
    AnchoredCube(Vec3<GLfloat> pos, Vec3<GLfloat> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Cube(pos, dim, gravity, color, texture, material) { }

    Vec3<GLfloat> getPosition() const {
        return transform.position + dim / 2.0f;
    }
};

class RidgedWall : public Object {
    Vec3<GLfloat> dim;
    Vec3<GLfloat> protrusionVec;

    GLint fixedCount = 0;
    GLfloat fixedSpacing = 0.0f;
    bool useFixedSpacing = false;
    GLfloat ridgeThickness;

    void generateStructure();
public:
    RidgedWall(Vec3<GLfloat> pos, Vec3<GLfloat> dim, Vec3<GLfloat> protrusionVec, GLint count, GLfloat ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim), protrusionVec(protrusionVec), fixedCount(count), ridgeThickness(ridgeThickness) {
        this->useFixedSpacing = false;
        generateStructure();
    }

    RidgedWall(Vec3<GLfloat> pos, Vec3<GLfloat> dim, Vec3<GLfloat> protrusionVec, GLfloat spacing, GLfloat ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim), protrusionVec(protrusionVec), fixedSpacing(spacing), ridgeThickness(ridgeThickness) {
        this->useFixedSpacing = true;
        generateStructure();
    }

    Vec3<GLfloat> getDimensions() const {
        return dim;
    }
};

class TriangularRidgedWall : public Object {
    Vec3<GLfloat> dim;
    Vec3<GLfloat> protrusionVec;

    GLfloat spacing;
    GLfloat ridgeThickness;
    GLfloat wallThickness;
    void generateStructure();
public:
    TriangularRidgedWall(Vec3<GLfloat> pos, Vec3<GLfloat> dim, Vec3<GLfloat> protrusionVec, GLfloat spacing, GLfloat ridgeThickness, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim), protrusionVec(protrusionVec), spacing(spacing), ridgeThickness(ridgeThickness) {
        generateStructure();
    }

    Vec3<GLfloat> getDimensions() const {
        return dim;
    }
};

class TriangularPrism : public Object {
protected:
    Vec3<GLfloat> dim;
    void drawInternal() override;
public:
    TriangularPrism(Vec3<float> pos, Vec3<float> dim, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
        : Object(pos, gravity, color, texture, material), dim(dim) { setScale(dim); }

    Vec3<GLfloat> getDimensions() { return dim; }
};

class FramedWindow : public Block {
    GLfloat totalWidth;
    GLfloat totalHeight;
    GLfloat totalLength;

    GLint rows;
    GLint columns;
    GLfloat windowWidth;
    GLfloat windowHeight;
    GLfloat frameSize;
    GLfloat spacing;
public:
    FramedWindow(Vec3<GLfloat> pos, GLint rows, GLint colums, GLfloat windowWidth, GLfloat windowHeight, GLfloat frameSize = 0.1f, GLfloat spacing = 0.05f,
        GLfloat scale = 1.0f, bool gravity = DEFAULT_GRAVITY, Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, MaterialID material = DEFAULT_MATERIAL)
    : Block(pos, scale, gravity, color, texture, material), rows(rows), columns(colums), windowWidth(windowWidth), windowHeight(windowHeight), frameSize(frameSize), spacing(spacing) { performScaling(); FramedWindow::updateDimensions(); FramedWindow::addAll(); }

    Vec3<GLfloat> getDimensions() override {
        return {totalWidth, totalHeight, totalLength};
    }
protected:
    void addAll() override;

    std::vector<GLfloat *> getScalableVars() override {
        return {
            &windowWidth,
            &windowHeight,
            &frameSize,
            &spacing
        };
    }

    void updateDimensions() override {
        totalWidth = frameSize * 2 + columns * windowWidth + (columns - 1) * spacing;
        totalHeight = frameSize * 2 + rows * windowHeight + (rows - 1) * spacing;
        totalLength = frameSize;
    }
};

class BorderCubes : public Object {
public:
    static constexpr float CUBE_SIZE = 0.1f;
    BorderCubes(Vec3<GLfloat> startPos, Vec3<GLfloat> dimensions)
        : Object(startPos, false, DEFAULT_COLOR, TextureID::NONE, MaterialID::MATTE) {
        createCorners(dimensions.x, dimensions.y, dimensions.z);
    }
    BorderCubes(Block *block) : BorderCubes(block->getWorldPosition(), block->getDimensions()) {}
    BorderCubes(AnchoredCube *cube) : BorderCubes(cube->getWorldPosition(), cube->getDimensions()) {}
    BorderCubes(AnchoredCuboid *cuboid) : BorderCubes(cuboid->getWorldPosition(), cuboid->getDimensions()) {}
private:
    void createCorners(float w, float h, float l);
    void drawInternal() override {}
};

class Rose : public Object {
public:
    Rose(Vec3<GLfloat> pos) : Object(pos) {
        // --- CONFIGURATION ---
        const float STEM_W = 0.25f;
        const float SEG_H = 1.3f;
        const float LEAF_L = 0.6f;
        const float LEAF_W = 0.4f;
        const float LEAF_T = 0.02f;

        const float BRANCH_L = 0.15f;
        const float BRANCH_T = 0.08f;

        const float P_W = 0.5f;
        const float P_H = 0.6f;
        const float P_T = 0.05f;
        const float R_IN = 0.15f;
        const float R_MID = 0.3f;
        const float R_OUT = 0.5f;

        Color3f white = {1.0f, 1.0f, 1.0f};
        Color3f red   = {0.8f, 0.0f, 0.1f};

        // --- ROSE ROOT ---
        Object* root = new Object(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
        root->setScale(Vec3<GLfloat>(0.2f, 0.2f, 0.2f));
        this->addChildren(root);

        // ==========================
        // STEM SEGMENT 1
        // ==========================
        Object* seg1 = new Object(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
        seg1->setRotation(10.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
        root->addChildren(seg1);

        // ==========================
        // STEM SEGMENT 2
        // ==========================
        Object* seg2 = new Object(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
        seg2->setRotation(15.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
        seg1->addChildren(seg2);

        // Visible Stem
        seg2->addChildren(new AnchoredCuboid(
            Vec3<GLfloat>(-STEM_W/2.0f, 0.0f, -STEM_W/2.0f),
            Vec3<GLfloat>(STEM_W, SEG_H, STEM_W),
            false, white, TextureID::LEAVES
        ));

        // --- LEAF ASSEMBLY 3 (Bottom) ---
        {
            Object* leafGroup = new Object(Vec3<GLfloat>(0.0f, SEG_H * 0.9f, 0.0f));
            leafGroup->setRotation(200.0f, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
            seg2->addChildren(leafGroup);

            Object* stemConnector = new Object(Vec3<GLfloat>(STEM_W / 2.0f, 0.0f, 0.0f));
            stemConnector->setRotation(10.0f, Vec3<GLfloat>(0.0f, 1.0f, 1.0f));
            stemConnector->setScale(Vec3<GLfloat>(0.6f, 0.6f, 0.6f));
            leafGroup->addChildren(stemConnector);

            // Branch
            stemConnector->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -BRANCH_T/2.0f, -BRANCH_T/2.0f),
                Vec3<GLfloat>(BRANCH_L, BRANCH_T, BRANCH_T),
                false, white, TextureID::LEAVES
            ));

            Object* pivot = new Object(Vec3<GLfloat>(BRANCH_L, 0.0f, 0.0f));
            pivot->setRotation(25.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
            stemConnector->addChildren(pivot);

            AnchoredCuboid* base = new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -LEAF_W/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W),
                false, white, TextureID::LEAVES
            );
            base->setRotation(8.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(base);

            Object* tipPivot = new Object(Vec3<GLfloat>(LEAF_L, 0.0f, 0.0f));
            tipPivot->setRotation(-16.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(tipPivot);

            tipPivot->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -(LEAF_W * 0.8f)/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W * 0.8f),
                false, white, TextureID::LEAVES
            ));
        }

        // --- LEAF ASSEMBLY 2 (Left) ---
        {
            Object* leafGroup = new Object(Vec3<GLfloat>(0.0f, SEG_H, 0.0f));
            leafGroup->setRotation(60.0f, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
            seg2->addChildren(leafGroup);

            Object* stemConnector = new Object(Vec3<GLfloat>(STEM_W / 2.0f, 0.0f, 0.0f));
            stemConnector->setScale(Vec3<GLfloat>(0.7f, 0.7f, 0.7f));
            leafGroup->addChildren(stemConnector);

            stemConnector->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -BRANCH_T/2.0f, -BRANCH_T/2.0f),
                Vec3<GLfloat>(BRANCH_L, BRANCH_T, BRANCH_T),
                false, white, TextureID::LEAVES
            ));

            Object* pivot = new Object(Vec3<GLfloat>(BRANCH_L, 0.0f, 0.0f));
            pivot->setRotation(25.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
            stemConnector->addChildren(pivot);

            AnchoredCuboid* base = new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -LEAF_W/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W),
                false, white, TextureID::LEAVES
            );
            base->setRotation(8.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(base);

            Object* tipPivot = new Object(Vec3<GLfloat>(LEAF_L, 0.0f, 0.0f));
            tipPivot->setRotation(-16.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(tipPivot);

            tipPivot->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -(LEAF_W * 0.8f)/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W * 0.8f),
                false, white, TextureID::LEAVES
            ));
        }

        // ==========================
        // STEM SEGMENT 3 (Top)
        // ==========================
        Object* seg3 = new Object(Vec3<GLfloat>(0.0f, SEG_H, 0.0f));
        seg3->setRotation(-20.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
        seg2->addChildren(seg3);

        seg3->addChildren(new AnchoredCuboid(
            Vec3<GLfloat>(-STEM_W/2.0f, 0.0f, -STEM_W/2.0f),
            Vec3<GLfloat>(STEM_W, SEG_H, STEM_W),
            false, white, TextureID::LEAVES
        ));

        // --- LEAF ASSEMBLY 1 (Top) ---
        {
            Object* leafGroup = new Object(Vec3<GLfloat>(0.0f, SEG_H * 0.4f, 0.0f));
            leafGroup->setRotation(-60.0f, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
            seg3->addChildren(leafGroup);

            Object* stemConnector = new Object(Vec3<GLfloat>(STEM_W / 2.0f, 0.0f, 0.0f));
            stemConnector->setScale(Vec3<GLfloat>(0.6f, 0.6f, 0.6f));
            leafGroup->addChildren(stemConnector);

            stemConnector->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -BRANCH_T/2.0f, -BRANCH_T/2.0f),
                Vec3<GLfloat>(BRANCH_L, BRANCH_T, BRANCH_T),
                false, white, TextureID::LEAVES
            ));

            Object* pivot = new Object(Vec3<GLfloat>(BRANCH_L, 0.0f, 0.0f));
            pivot->setRotation(-5.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
            stemConnector->addChildren(pivot);

            AnchoredCuboid* base = new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -LEAF_W/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W),
                false, white, TextureID::LEAVES
            );
            base->setRotation(8.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(base);

            Object* tipPivot = new Object(Vec3<GLfloat>(LEAF_L, 0.0f, 0.0f));
            tipPivot->setRotation(-16.0f, Vec3<GLfloat>(0.0f, 0.0f, 1.0f));
            pivot->addChildren(tipPivot);

            tipPivot->addChildren(new AnchoredCuboid(
                Vec3<GLfloat>(0.0f, -LEAF_T/2.0f, -(LEAF_W * 0.8f)/2.0f),
                Vec3<GLfloat>(LEAF_L, LEAF_T, LEAF_W * 0.8f),
                false, white, TextureID::LEAVES
            ));
        }

        // --- FLOWER HEAD ---
        Object* flowerHead = new Object(Vec3<GLfloat>(0.0f, SEG_H, 0.0f));
        seg3->addChildren(flowerHead);

        // Center Cube (Anchored Logic)
        AnchoredCube* center = new AnchoredCube(
            Vec3<GLfloat>(-0.15f, -0.05f, -0.15f), // Offset to center at (0, 0.1, 0)
            0.3f,
            false, red, TextureID::NONE
        );
        center->setRotation(90.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
        flowerHead->addChildren(center);

        // --- PETAL GENERATOR ---
        auto addPetalLayer = [&](int count, float radius, float width, float tilt) {
            float angleStep = 360.0f / count;
            for (int i = 0; i < count; i++) {
                float angle = i * angleStep;

                // 1. Azimuth Pivot (Rotate around Flower Center Y)
                Object* azimuthPivot = new Object(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
                azimuthPivot->setRotation(angle, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
                flowerHead->addChildren(azimuthPivot);

                // 2. Petal Pivot (Translate to Radius, then Tilt)
                // We create a container at the petal's center position.
                // This allows us to tilt it around ITS center.
                Object* petalPivot = new Object(Vec3<GLfloat>(0.0f, 0.0f, radius));
                petalPivot->setRotation(tilt, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
                azimuthPivot->addChildren(petalPivot);

                // 3. The Visual Petal (AnchoredCuboid)
                // Since AnchoredCuboid's origin is the corner, we offset it by (-W/2, -H/2, -T/2)
                // so that the box appears centered on 'petalPivot'.
                AnchoredCuboid* petal = new AnchoredCuboid(
                    Vec3<GLfloat>(-width/2.0f, -P_H/2.0f, -P_T/2.0f),
                    Vec3<GLfloat>(width, P_H, P_T),
                    false, red, TextureID::NONE
                );
                petalPivot->addChildren(petal);
            }
        };

        // LAYER 1: INNER
        addPetalLayer(3, R_IN, P_W, 15.0f);

        // LAYER 2: MIDDLE
        float midWidth = P_W * 1.2f;
        for (int i = 0; i < 5; i++) {
            float angle = 36.0f + (i * 72.0f);

            Object* azimuthPivot = new Object(Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
            azimuthPivot->setRotation(angle, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
            flowerHead->addChildren(azimuthPivot);

            Object* petalPivot = new Object(Vec3<GLfloat>(0.0f, 0.0f, R_MID));
            petalPivot->setRotation(35.0f, Vec3<GLfloat>(1.0f, 0.0f, 0.0f));
            azimuthPivot->addChildren(petalPivot);

            AnchoredCuboid* petal = new AnchoredCuboid(
                Vec3<GLfloat>(-midWidth/2.0f, -P_H/2.0f, -P_T/2.0f),
                Vec3<GLfloat>(midWidth, P_H, P_T),
                false, red, TextureID::NONE
            );
            petalPivot->addChildren(petal);
        }

        // LAYER 3: OUTER
        float outWidth = P_W * 1.5f;
        addPetalLayer(6, R_OUT, outWidth, 60.0f);
    }

protected:
    void drawInternal() override {}
};

#endif //SRC_BUILDINGBLOCKS_H