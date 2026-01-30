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

void TriangularRidgedWall::generateStructure() {
    // 1. Determine orientation and length
    // runDir is the direction the wall stretches along (perpendicular to up and protrusion)
    Vec3<GLfloat> up(0.0f, 1.0f, 0.0f);
    Vec3<GLfloat> runDir = protrusionVec.cross(up).abs();
    GLfloat wallLength = std::abs(dim.dot(runDir)); // e.g., dim.x if running along X

    // 2. Add Backing Prism
    // We position the prism at the center of the wall's bounding box [0..dim]
    Vec3<GLfloat> wallCenter = dim / 2.0f;

    // Determine thickness component (dimension parallel to protrusion)
    GLfloat thickness = std::abs(dim.dot(protrusionVec.abs().normalize()));

    // Construct Prism Dimensions: (Width, Height, Thickness)
    // By default, we assume the prism's triangular face is in the XY plane (Width x Height).
    // If the wall runs along Z, we swap dimensions and rotate it.
    bool runsAlongZ = (runDir.z > 0.5f);
    
    Vec3<GLfloat> prismDims;
    if (runsAlongZ) {
        // If running along Z, we create it as (Length, Height, Thickness) in local space
        // and rotate it 90 degrees so 'Length' aligns with World Z.
        prismDims = Vec3(wallLength, dim.y, thickness);
    } else {
        // If running along X, standard orientation matches.
        prismDims = Vec3(wallLength, dim.y, thickness);
    }

    TriangularPrism* backing = new TriangularPrism(
        wallCenter,
        prismDims,
        gravity,
        color,
        texture,
        material
    );

    if (runsAlongZ) {
        backing->setRotation(90.0f, up);
    }
    addChildren(backing);

    // 3. Add Ridges
    // Calculate count based on spacing (using member 'spacing' from header)
    // Default to 1.0f to avoid division by zero if uninitialized
    GLfloat effectiveSpacing = (spacing <= 0.01f) ? 1.0f : spacing;
    int count = static_cast<int>(wallLength / effectiveSpacing);
    
    GLfloat ridgeBreadth = effectiveSpacing * 0.2f;
    GLfloat firstOffset = effectiveSpacing / 2.0f;
    
    // Base size of ridge (X/Z components), Height (Y) will be calculated per ridge
    Vec3<GLfloat> baseRidgeSize = (runDir * ridgeBreadth).abs()
                                + (protrusionVec * ridgeThickness).abs();

    for (int i = 0; i < count; i++) {
        GLfloat offsetScalar = firstOffset + (i * effectiveSpacing);

        // --- Height Calculation ---
        // Calculate height based on distance from the center (Isosceles Triangle slope)
        // We calculate 'dist' using the center of the ridge to be precise.
        GLfloat centerOfWall = wallLength / 2.0f;
        GLfloat ridgeCenter = offsetScalar + (ridgeBreadth / 2.0f);
        GLfloat distFromMiddle = std::abs(ridgeCenter - centerOfWall);
        
        // Linear slope: Peak Height at center, 0 at edges.
        GLfloat h = dim.y * (1.0f - distFromMiddle / centerOfWall);
        
        // Clamp to ensure no negative height
        if (h < 0.0f) h = 0.0f;

        // --- Position Logic ---
        Vec3<GLfloat> posAlongWall = runDir * offsetScalar;
        Vec3<GLfloat> posPerpendicular(0.0f, 0.0f, 0.0f);

        // Logic to place ridge on the correct face (Front/Back/Left/Right)
        if (protrusionVec.x > 0)
            posPerpendicular.x = dim.x;
        else if (protrusionVec.x < 0)
            posPerpendicular.x = -ridgeThickness;

        if (protrusionVec.z > 0)
            posPerpendicular.z = dim.z;
        else if (protrusionVec.z < 0)
            posPerpendicular.z = -ridgeThickness;

        Vec3<GLfloat> finalPos = posAlongWall + posPerpendicular;

        // Apply calculated height
        Vec3<GLfloat> currentRidgeSize = baseRidgeSize;
        currentRidgeSize.y = h;

        AnchoredCube* ridge = new AnchoredCube(
            finalPos,
            currentRidgeSize,
            gravity,
            color,
            texture,
            material
        );

        addChildren(ridge);
    }
}

void TriangularPrism::drawInternal() {
    glColor3f(color.red, color.green, color.blue);

    float w = 0.5f; // x
    float h = 0.5f; // y
    float d = 0.5f; // z

    Vec3<float> top(0.0f, h, d);
    Vec3<float> botLeft(-w, -h, d);
    Vec3<float> botRight(w, -h, d);
    Vec3<float> topBack(0.0f, h, -d);
    Vec3<float> botLeftBack(-w, -h, -d);
    Vec3<float> botRightBack(w, -h, -d);

    Vec3<float> nFront(0, 0, 1);
    Vec3<float> nBack(0, 0, -1);
    Vec3<float> nBot(0, -1, 0);
    Vec3<float> nLeft = Vec3<float>(-h, w, 0).normalize();
    Vec3<float> nRight = Vec3<float>(h, w, 0).normalize();

    glBegin(GL_TRIANGLES);
    glNormal3f(nFront.x, nFront.y, nFront.z);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(botLeft.x, botLeft.y, botLeft.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botRight.x, botRight.y, botRight.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(nBack.x, nBack.y, nBack.z);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(topBack.x, topBack.y, topBack.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botRightBack.x, botRightBack.y, botRightBack.z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(botLeftBack.x, botLeftBack.y, botLeftBack.z);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(nBot.x, nBot.y, nBot.z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(botLeftBack.x, botLeftBack.y, botLeftBack.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botRightBack.x, botRightBack.y, botRightBack.z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(botRight.x, botRight.y, botRight.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(botLeft.x, botLeft.y, botLeft.z);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(nLeft.x, nLeft.y, nLeft.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(topBack.x, topBack.y, topBack.z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(botLeftBack.x, botLeftBack.y, botLeftBack.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botLeft.x, botLeft.y, botLeft.z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(top.x, top.y, top.z);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(nRight.x, nRight.y, nRight.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botRight.x, botRight.y, botRight.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(botRightBack.x, botRightBack.y, botRightBack.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(topBack.x, topBack.y, topBack.z);
    glEnd();
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
        Object* corner = new Cube(
            pos,
            Vec3(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE),
            gravity,
            color,
            TextureID::NONE,
            MaterialID::COLD_LIGHT
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
                MaterialID::MATTE,
                TextureConfig(),
                2
            );
            glassPane->setStatic(true);

            addChildren(glassPane);
        }
    }
}
