#ifndef SPLINE_H
#define SPLINE_H

#include "Object.h"
#include <vector>
#include <functional> 
#include <GL/glu.h>

class SplineObject : public Object {
protected:
    static GLUnurbsObj* nurbRenderer;
    static int referenceCount;

    int order;       
    int resolution;  

    std::vector<GLfloat> generateClampedKnots(int numControlPoints, int order);
    void initRenderer();
    void destroyRenderer();

public:
    SplineObject(Vec3<float> pos, int order = 4, int resolution = 25);
    virtual ~SplineObject();
    
    void setResolution(int res);
};

class NurbsCurve : public SplineObject {
private:
    std::vector<GLfloat> controlPointsFlat;
    std::vector<GLfloat> knots;
    int numPoints;

    void drawInternal() override;

public:
    NurbsCurve(std::vector<Vec3<float>> points, Vec3<float> pos = Vec3<float>(0,0,0),
        int order = 4, Color3f color = {1,1,1});

    NurbsCurve(std::function<Vec3<float>(float)> func, float minT, float maxT, int steps,
        Vec3<float> pos = Vec3<float>(0,0,0), int order = 4, Color3f color = {1,1,1});

    void updateControlPoints(const std::vector<Vec3<float>>& points);
};

class NurbsSurface : public SplineObject {
    TextureConfig texConfig;
private:
    std::vector<GLfloat> controlPointsFlat; 
    std::vector<GLfloat> uKnots;
    std::vector<GLfloat> vKnots;
    int uSize, vSize;

    void drawInternal() override;

public:
    NurbsSurface(const std::vector<std::vector<Vec3<float>>>& grid, Vec3<float> pos = Vec3<float>(0,0,0),
        int order = 4, Color3f color = {1,1,1},TextureID texture = TextureID::NONE,MaterialID material = MaterialID::NONE);

    NurbsSurface(std::function<Vec3<float>(float, float)> func, float minU, float maxU, int uSteps, float minV, float maxV, int vSteps,
        Vec3<float> pos = Vec3<float>(0,0,0), int order = 4, Color3f color = {1,1,1}, TextureID texture = TextureID::NONE, MaterialID material = MaterialID::NONE);
    
    void setTextureConfig(TextureConfig config) override {
        this->texConfig = config;
    }
};

#endif