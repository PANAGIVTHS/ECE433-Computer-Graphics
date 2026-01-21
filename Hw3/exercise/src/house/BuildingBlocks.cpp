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
    GLfloat spacing = wallLength / static_cast<float>(count);
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