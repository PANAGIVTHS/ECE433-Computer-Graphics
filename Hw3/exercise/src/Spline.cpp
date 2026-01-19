#include "Spline.h"
#include "TextureManager.h"
#include <iostream>

GLUnurbsObj* SplineObject::nurbRenderer = nullptr;
int SplineObject::referenceCount = 0;

SplineObject::SplineObject(Vec3<float> pos, int order, int resolution)
    : Object(pos), order(order), resolution(resolution) {
    initRenderer();
}

SplineObject::~SplineObject() {
    destroyRenderer();
}

void SplineObject::initRenderer() {
    if (referenceCount == 0) {
        nurbRenderer = gluNewNurbsRenderer();
        gluNurbsProperty(nurbRenderer, GLU_SAMPLING_TOLERANCE, 25.0);
        gluNurbsProperty(nurbRenderer, GLU_DISPLAY_MODE, GLU_FILL);
        gluNurbsProperty(nurbRenderer, GLU_AUTO_LOAD_MATRIX, GL_FALSE);
    }
    //! Maybe at some point better to have more?
    referenceCount++;
}

void SplineObject::destroyRenderer() {
    referenceCount--;
    if (referenceCount <= 0 && nurbRenderer) {
        gluDeleteNurbsRenderer(nurbRenderer);
        nurbRenderer = nullptr;
    }
}

void SplineObject::setResolution(int res) {
    this->resolution = res;
}

std::vector<GLfloat> SplineObject::generateClampedKnots(int numControlPoints, int order) {
    int numKnots = numControlPoints + order;
    std::vector<GLfloat> knots(numKnots);
    for (int i = 0; i < order; i++) knots[i] = 0.0f;
    for (int i = numKnots - order; i < numKnots; i++) knots[i] = 1.0f;
    int internalKnots = numKnots - 2 * order;
    if (internalKnots > 0) {
        float step = 1.0f / (internalKnots + 1);
        for (int i = 0; i < internalKnots; i++) {
            knots[order + i] = step * (i + 1);
        }
    }
    return knots;
}

NurbsCurve::NurbsCurve(std::vector<Vec3<float>> points, Vec3<float> pos, int order, Color3f color)
    : SplineObject(pos, order) {
    this->color = color;
    updateControlPoints(points);
}

NurbsCurve::NurbsCurve(std::function<Vec3<float>(float)> func, float minT, float maxT, int steps, Vec3<float> pos, int order, Color3f color)
    : SplineObject(pos, order) {
    
    this->color = color;
    std::vector<Vec3<float>> points;

    if (steps < 2) steps = 2; // Safety check
    
    float stepSize = (maxT - minT) / (float)(steps - 1);
    
    for(int i = 0; i < steps; i++) {
        float t = minT + i * stepSize;
        points.push_back(func(t));
    }

    updateControlPoints(points);
}

void NurbsCurve::updateControlPoints(const std::vector<Vec3<float>>& points) {
    this->numPoints = points.size();
    controlPointsFlat.clear();
    for (const auto& p : points) {
        controlPointsFlat.push_back(p.x);
        controlPointsFlat.push_back(p.y);
        controlPointsFlat.push_back(p.z);
    }
    this->knots = generateClampedKnots(numPoints, order);
}

void NurbsCurve::drawInternal() {
    glColor3f(color.red, color.green, color.blue);
    glDisable(GL_LIGHTING); 
    glLineWidth(3.0f);

    gluBeginCurve(nurbRenderer);
    gluNurbsCurve(nurbRenderer, knots.size(), &knots[0], 3, &controlPointsFlat[0], order, GL_MAP1_VERTEX_3);
    gluEndCurve(nurbRenderer);

    glEnable(GL_LIGHTING); 
}

NurbsSurface::NurbsSurface(const std::vector<std::vector<Vec3<float>>>& grid, Vec3<float> pos, int order, Color3f color, TextureID texture, MaterialID material)
    : SplineObject(pos, order) {
    this->color = color;
    this->texture = texture;
    this->material = material;

    if (grid.empty() || grid[0].empty()) return;

    uSize = grid.size();   
    vSize = grid[0].size(); 

    for (const auto& row : grid) {
        for (const auto& p : row) {
            controlPointsFlat.push_back(p.x);
            controlPointsFlat.push_back(p.y);
            controlPointsFlat.push_back(p.z);
        }
    }
    uKnots = generateClampedKnots(uSize, order);
    vKnots = generateClampedKnots(vSize, order);
}

NurbsSurface::NurbsSurface(std::function<Vec3<float>(float, float)> func,
                           float minU, float maxU, int uSteps,
                           float minV, float maxV, int vSteps,
                           Vec3<float> pos, int order, Color3f color,
                           TextureID texture, MaterialID material)
    : SplineObject(pos, order) {
    
    this->color = color;
    this->texture = texture;
    this->material = material;

    uSize = uSteps;
    vSize = vSteps;

    float uStepSize = (maxU - minU) / (float)(uSteps - 1);
    float vStepSize = (maxV - minV) / (float)(vSteps - 1);

    for(int i = 0; i < uSteps; i++) {
        float u = minU + i * uStepSize;
        for(int j = 0; j < vSteps; j++) {
            float v = minV + j * vStepSize;
            
            Vec3<float> p = func(u, v);
            
            controlPointsFlat.push_back(p.x);
            controlPointsFlat.push_back(p.y);
            controlPointsFlat.push_back(p.z);
        }
    }

    uKnots = generateClampedKnots(uSize, order);
    vKnots = generateClampedKnots(vSize, order);
}

void NurbsSurface::drawInternal() {
    gluNurbsProperty(nurbRenderer, GLU_SAMPLING_TOLERANCE, (GLfloat)resolution);
    gluNurbsProperty(nurbRenderer, GLU_DISPLAY_MODE, GLU_FILL); 

    MaterialManager::bind(material);
    TextureManager::bind(texture); 
    glColor3f(color.red, color.green, color.blue);

    gluBeginSurface(nurbRenderer);
    gluNurbsSurface(
        nurbRenderer,
        uKnots.size(), &uKnots[0],
        vKnots.size(), &vKnots[0],
        vSize * 3, //! u_stride
        3,         //! v_stride
        &controlPointsFlat[0],
        order, order,
        GL_MAP2_VERTEX_3
    );
    gluEndSurface(nurbRenderer);
}