#include "BuildingBlocks.h"

void RidgedWall::generateStructure() {
    AnchoredCube* baseWall = new AnchoredCube(
        Vec3(0.0f, 0.0f, 0.0f),
        dim,
        gravity,
        color,
        texture,
        material
    );
    addChildren(baseWall);

    Vec3<GLfloat> runDir = protrusionVec.cross(Vec3(0.0f, 1.0f, 0.0f)).abs();
    GLfloat wallLength = std::abs(dim.dot(runDir));
    
    GLfloat spacing;
    GLint count;

    if (useFixedSpacing) {
        // Distance Mode: Spacing is fixed, Count is calculated
        spacing = fixedSpacing;
        if (spacing == 0.0f) spacing = 1.0f;
        count = static_cast<int>(wallLength / spacing);
    } else {
        // Count Mode: Count is fixed, Spacing is calculated
        count = fixedCount;
        spacing = (count >= 1) ? wallLength / static_cast<float>(count) : 1;
    }
    
    GLfloat ridgeBreadth = spacing * 0.2f;
    Vec3<GLfloat> ridgeSize = (runDir * ridgeBreadth).abs()
                            + Vec3(0.0f, dim.y, 0.0f)
                            + (protrusionVec * ridgeThickness).abs();

    GLfloat firstOffset = spacing / 2.0f;
    for (int i = 0; i < count; i++) {
        GLfloat offsetScalar = firstOffset + (i * spacing);
        Vec3<GLfloat> posAlongWall = runDir * offsetScalar;
        Vec3<GLfloat> posPerpendicular(0.0f, 0.0f, 0.0f);

        if (protrusionVec.x > 0)
            posPerpendicular.x = dim.x;
        else if (protrusionVec.x < 0)
            posPerpendicular.x = -ridgeThickness;

        if (protrusionVec.z > 0)
            posPerpendicular.z = dim.z;
        else if (protrusionVec.z < 0)
            posPerpendicular.z = -ridgeThickness;

        Vec3<GLfloat> finalPos = posAlongWall + posPerpendicular;

        AnchoredCube* ridge = new AnchoredCube(
            finalPos,
            ridgeSize,
            gravity,
            color,
            texture,
            material
        );

        addChildren(ridge);
    }
}

void GlowingCube::drawInternal() {
    glDisable(GL_LIGHTING);
    Cube::drawInternal();
    glEnable(GL_LIGHTING);
}

void BorderCubes::createCorners(float w, float h, float l) {
    std::vector<Vec3<float>> positions = {
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(w,    0.0f, 0.0f),
        Vec3(0.0f, h,    0.0f),
        Vec3(0.0f, 0.0f, l),
        Vec3(w,    h,    0.0f),
        Vec3(w,    0.0f, l),
        Vec3(0.0f, h,    l),
        Vec3(w,    h,    l)
    };

    for (const auto& pos : positions) {
        Color3f color = {.red = 1, .green = 1, .blue = 1};
        if (pos.x == 0 && pos.y == 0 && pos.z == 0) color = {.red = 1, .green = 0, .blue = 0};
        Object* corner = new GlowingCube(
            pos,
            Vec3(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE),
            gravity,
            color,
            TextureID::NONE,
            MaterialID::MATTE
        );
        addChildren(corner);
    }
}

void FramedWindow::addAll() {
    // Vertical
    AnchoredCube* firstBar = new AnchoredCube(
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(frameSize, totalHeight, frameSize),
        gravity,
        color,
        texture,
        material
    );
    addChildren(firstBar);

    for (int i = 1; i < columns; i++) {
        GLfloat centerX = frameSize + i * windowWidth + (i - 1) * spacing;
        GLfloat centerZ = frameSize * 0.25f;

        AnchoredCube* vBar = new AnchoredCube(
            Vec3(centerX, frameSize, centerZ),
            Vec3(spacing, totalHeight - 2 * frameSize, spacing),
            gravity,
            color,
            texture,
            material
        );
        addChildren(vBar);
    }

    AnchoredCube* lastBar = new AnchoredCube(
        Vec3(frameSize + columns * windowWidth + (columns - 1) * spacing, 0.f, 0.0f),
        Vec3(frameSize, totalHeight, frameSize),
        gravity,
        color,
        texture,
        material
    );
    addChildren(lastBar);

    // Horizontal
    Object* bottomBar = new AnchoredCube(
        Vec3(frameSize, 0.0f, 0.0f),
        Vec3(columns * windowWidth + (columns - 1) * spacing, frameSize, frameSize),
        gravity,
        color,
        texture,
        material
    );
    addChildren(bottomBar);

    for (int r = 1; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            GLfloat centerX = frameSize + c * (windowWidth + spacing);
            GLfloat centerY = frameSize + r * windowHeight + (r - 1) * spacing;
            GLfloat centerZ = frameSize * 0.25f;

            Object* hBar = new AnchoredCube(
                Vec3(centerX, centerY, centerZ),
                Vec3(windowWidth, spacing, spacing),
                gravity,
                color,
                texture,
                material
            );
            addChildren(hBar);
        }
    }

    Object* topBar = new AnchoredCube(
        Vec3(frameSize, totalHeight - frameSize, 0.0f),
        Vec3(columns * windowWidth + (columns - 1) * spacing, frameSize, frameSize),
        gravity,
        color,
        texture,
        material
    );
    addChildren(topBar);

    // ---------------------------------------------------------
    // WINDOWS
    // ---------------------------------------------------------
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            GLfloat centerX = frameSize + c * (windowWidth + spacing);
            GLfloat centerY = frameSize + r * (windowHeight + spacing);

            Object* glassPane = new AnchoredCuboid(
                Vec3(centerX, centerY, frameSize * 0.25f + spacing * 0.25f),
                Vec3(windowWidth, windowHeight, spacing * 0.5f),
                gravity,
                color,
                TextureID::WINDOW,
                MaterialID::SHINY,
                TextureConfig(),
                2
            );
            glassPane->optimize();

            addChildren(glassPane);
        }
    }
}
