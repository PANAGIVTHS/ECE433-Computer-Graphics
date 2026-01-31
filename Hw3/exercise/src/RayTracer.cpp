#include "RayTracer.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include "Celestial.h" 
#include "LightingManager.h" // Added

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

bool RayTracer::enabled = false;
std::vector<unsigned char> RayTracer::pixelBuffer;

// ==========================================
//           FAST MATH & STRUCTURES
// ==========================================

struct Mat4 {
    float m[16]; 
    static Mat4 identity() { return {{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}}; }
    Mat4 operator*(const Mat4& r) const {
        Mat4 res;
        for(int c=0; c<4; c++) {
            for(int r_idx=0; r_idx<4; r_idx++) {
                float sum = 0;
                sum += m[0*4 + r_idx] * r.m[c*4 + 0];
                sum += m[1*4 + r_idx] * r.m[c*4 + 1];
                sum += m[2*4 + r_idx] * r.m[c*4 + 2];
                sum += m[3*4 + r_idx] * r.m[c*4 + 3];
                res.m[c*4 + r_idx] = sum;
            }
        }
        return res;
    }
    Vec3<float> transformPoint(const Vec3<float>& v) const {
        return {
            m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12],
            m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13],
            m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]
        };
    }
    Vec3<float> transformDir(const Vec3<float>& v) const {
        return {
            m[0]*v.x + m[4]*v.y + m[8]*v.z,
            m[1]*v.x + m[5]*v.y + m[9]*v.z,
            m[2]*v.x + m[6]*v.y + m[10]*v.z
        };
    }
};

Mat4 makeTranslate(Vec3<float> t) { Mat4 r = Mat4::identity(); r.m[12]=t.x; r.m[13]=t.y; r.m[14]=t.z; return r; }
Mat4 makeScale(Vec3<float> s) { Mat4 r = Mat4::identity(); r.m[0]=s.x; r.m[5]=s.y; r.m[10]=s.z; return r; }
Mat4 makeInvTranslate(Vec3<float> t) { Mat4 r = Mat4::identity(); r.m[12]=-t.x; r.m[13]=-t.y; r.m[14]=-t.z; return r; }
Mat4 makeInvScale(Vec3<float> s) { 
    Mat4 r = Mat4::identity(); 
    r.m[0]=1.0f/(std::abs(s.x)<1e-4?1e-4:s.x); 
    r.m[5]=1.0f/(std::abs(s.y)<1e-4?1e-4:s.y); 
    r.m[10]=1.0f/(std::abs(s.z)<1e-4?1e-4:s.z); 
    return r; 
}
Mat4 makeRotate(float angle, Vec3<float> axis) {
    if (std::abs(angle) < 1e-4) return Mat4::identity();
    float rad = angle * (M_PI / 180.0f);
    float c = cos(rad), s = sin(rad);
    float mag = sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    if(mag > 0) axis = axis / mag;
    float x=axis.x, y=axis.y, z=axis.z, C=1-c;
    Mat4 r;
    r.m[0] = x*x*C+c;   r.m[4] = x*y*C-z*s; r.m[8] = x*z*C+y*s; r.m[12]=0;
    r.m[1] = y*x*C+z*s; r.m[5] = y*y*C+c;   r.m[9] = y*z*C-x*s; r.m[13]=0;
    r.m[2] = z*x*C-y*s; r.m[6] = z*y*C+x*s; r.m[10]= z*z*C+c;   r.m[14]=0;
    r.m[3] = 0;         r.m[7] = 0;         r.m[11]= 0;         r.m[15]=1;
    return r;
}

struct AABB {
    Vec3<float> min = {1e9, 1e9, 1e9};
    Vec3<float> max = {-1e9, -1e9, -1e9};
    void expand(const Vec3<float>& p) {
        if(p.x < min.x) min.x = p.x; if(p.y < min.y) min.y = p.y; if(p.z < min.z) min.z = p.z;
        if(p.x > max.x) max.x = p.x; if(p.y > max.y) max.y = p.y; if(p.z > max.z) max.z = p.z;
    }
    float intersect(const Vec3<float>& origin, const Vec3<float>& invDir) const {
        float t1 = (min.x - origin.x) * invDir.x, t2 = (max.x - origin.x) * invDir.x;
        float tMin = std::max(std::min(t1, t2), -1e9f), tMax = std::min(std::max(t1, t2), 1e9f);
        t1 = (min.y - origin.y) * invDir.y; t2 = (max.y - origin.y) * invDir.y;
        tMin = std::max(tMin, std::min(t1, t2)); tMax = std::min(tMax, std::max(t1, t2));
        t1 = (min.z - origin.z) * invDir.z; t2 = (max.z - origin.z) * invDir.z;
        tMin = std::max(tMin, std::min(t1, t2)); tMax = std::min(tMax, std::max(t1, t2));
        return (tMax >= tMin && tMax > 0) ? tMin : 1e30f;
    }
    Vec3<float> center() const { return Vec3<float>((min.x+max.x)*0.5f, (min.y+max.y)*0.5f, (min.z+max.z)*0.5f); }
};

struct FastObj {
    Mat4 invWorld;      
    Mat4 worldRot;      
    Vec3<float> color;
    AABB bounds;        
    Vec3<float> centroid; 
};

struct FastLight {
    Vec3<float> pos;
    Vec3<float> color;
    float constant, linear, quadratic;
};

struct BVHNode {
    AABB box;
    int leftIndex = -1, rightIndex = -1;
    int objStart = 0, objCount = 0;
};

// Global Rendering Data
std::vector<FastObj> primitives;
std::vector<int> primitiveIndices;
std::vector<BVHNode> bvhNodes;
std::vector<FastLight> sceneLights; // Flattened lights
static std::vector<Object*> renderQueue; 

// ==========================================
//           BVH BUILDER
// ==========================================

void updateNodeBounds(int nodeIdx) {
    BVHNode& node = bvhNodes[nodeIdx];
    node.box = AABB();
    for (int i = 0; i < node.objCount; ++i) {
        int objIdx = primitiveIndices[node.objStart + i];
        const AABB& objBox = primitives[objIdx].bounds;
        node.box.expand(objBox.min);
        node.box.expand(objBox.max);
    }
}

void splitBVH(int nodeIdx) {
    if (bvhNodes[nodeIdx].objCount <= 2) return; 

    const BVHNode& node = bvhNodes[nodeIdx];
    Vec3<float> extent = node.box.max - node.box.min;
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent.y && extent.z > extent.x) axis = 2;

    float splitPos = (axis == 0) ? node.box.center().x : (axis == 1) ? node.box.center().y : node.box.center().z;

    int i = node.objStart;
    int j = node.objStart + node.objCount - 1;
    while (i <= j) {
        float pos = (axis == 0) ? primitives[primitiveIndices[i]].centroid.x :
                    (axis == 1) ? primitives[primitiveIndices[i]].centroid.y :
                                  primitives[primitiveIndices[i]].centroid.z;
        if (pos < splitPos) i++;
        else {
            std::swap(primitiveIndices[i], primitiveIndices[j]);
            j--;
        }
    }

    int leftCount = i - node.objStart;
    int currentObjCount = bvhNodes[nodeIdx].objCount;
    
    if (leftCount == 0 || leftCount == currentObjCount) return; 

    int rightCount = currentObjCount - leftCount;
    int currentStart = bvhNodes[nodeIdx].objStart;

    int leftChildIdx = bvhNodes.size();
    bvhNodes.push_back(BVHNode());
    int rightChildIdx = bvhNodes.size();
    bvhNodes.push_back(BVHNode());

    bvhNodes[nodeIdx].leftIndex = leftChildIdx;
    bvhNodes[nodeIdx].rightIndex = rightChildIdx;
    bvhNodes[nodeIdx].objCount = 0; 

    bvhNodes[leftChildIdx].objStart = currentStart;
    bvhNodes[leftChildIdx].objCount = leftCount;
    
    bvhNodes[rightChildIdx].objStart = i; 
    bvhNodes[rightChildIdx].objCount = rightCount; 

    updateNodeBounds(leftChildIdx);
    updateNodeBounds(rightChildIdx);
    splitBVH(leftChildIdx);
    splitBVH(rightChildIdx);
}

// ==========================================
//           INTERSECTION LOGIC
// ==========================================

bool intersectUnitBox(const Vec3<float>& origin, const Vec3<float>& dir, float& tOut, Vec3<float>& nOut) {
    float tMin = -1e9, tMax = 1e9;
    Vec3<float> minB(-0.5f, -0.5f, -0.5f), maxB(0.5f, 0.5f, 0.5f);
    float invDx = 1.0f/dir.x, invDy = 1.0f/dir.y, invDz = 1.0f/dir.z;

    float t1 = (minB.x - origin.x) * invDx, t2 = (maxB.x - origin.x) * invDx;
    tMin = std::max(tMin, std::min(t1, t2)); tMax = std::min(tMax, std::max(t1, t2));
    t1 = (minB.y - origin.y) * invDy; t2 = (maxB.y - origin.y) * invDy;
    tMin = std::max(tMin, std::min(t1, t2)); tMax = std::min(tMax, std::max(t1, t2));
    t1 = (minB.z - origin.z) * invDz; t2 = (maxB.z - origin.z) * invDz;
    tMin = std::max(tMin, std::min(t1, t2)); tMax = std::min(tMax, std::max(t1, t2));

    if (tMin <= tMax && tMax >= 0) {
        tOut = (tMin > 0) ? tMin : tMax;
        Vec3<float> p = origin + dir * tOut;
        float eps = 1e-3f;
        if (std::abs(p.x - minB.x) < eps) nOut = {-1, 0, 0};
        else if (std::abs(p.x - maxB.x) < eps) nOut = {1, 0, 0};
        else if (std::abs(p.y - minB.y) < eps) nOut = {0, -1, 0};
        else if (std::abs(p.y - maxB.y) < eps) nOut = {0, 1, 0};
        else if (std::abs(p.z - minB.z) < eps) nOut = {0, 0, -1};
        else nOut = {0, 0, 1};
        return true;
    }
    return false;
}

void traceBVH(const Vec3<float>& rayOrg, const Vec3<float>& rayDir, float& closestT, int& hitIndex, Vec3<float>& hitNormal, bool shadowMode) {
    int stack[128]; int stackPtr = 0; stack[stackPtr++] = 0; 
    Vec3<float> invDir = {1.0f/rayDir.x, 1.0f/rayDir.y, 1.0f/rayDir.z};

    while (stackPtr > 0) {
        int nodeIdx = stack[--stackPtr];
        const BVHNode& node = bvhNodes[nodeIdx];
        if (node.box.intersect(rayOrg, invDir) >= closestT) continue;

        if (node.leftIndex == -1) { 
            for (int i = 0; i < node.objCount; ++i) {
                int objIdx = primitiveIndices[node.objStart + i];
                const FastObj& obj = primitives[objIdx];
                Vec3<float> lOrg = obj.invWorld.transformPoint(rayOrg);
                Vec3<float> lDir = obj.invWorld.transformDir(rayDir);
                float t = 0; Vec3<float> n;
                if (intersectUnitBox(lOrg, lDir, t, n)) {
                    if (t < closestT && t > 0.001f) {
                        if (shadowMode) { closestT = t; return; }
                        closestT = t; hitIndex = objIdx; hitNormal = n;
                    }
                }
            }
        } else {
            stack[stackPtr++] = node.leftIndex;
            stack[stackPtr++] = node.rightIndex;
        }
    }
}

// ==========================================
//           MAIN RENDER FUNCTION
// ==========================================

void RayTracer::render(int screenWidth, int screenHeight) {
    if (!enabled) return;

    int scale = 14; // 1/20th Resolution
    int w = screenWidth / scale;
    int h = screenHeight / scale;
    if (w <= 0 || h <= 0) return;

    if (pixelBuffer.size() != w * h * 3) pixelBuffer.resize(w * h * 3);

    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    GameManager::getCamera()->set(); 
    GLdouble modelview[16], projection[16]; GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glPopMatrix(); 

    // --- 1. COLLECT LIGHTS (Flattened) ---
    sceneLights.clear();
    // A. Sun (Explicit)
    if (GameManager::getSun()) {
        FastLight sun;
        sun.pos = GameManager::getSun()->getWorldPosition();
        sun.color = {1.0f, 1.0f, 1.0f}; // Sun color
        sun.constant = 1.0f; sun.linear = 0.0f; sun.quadratic = 0.0f;
        sceneLights.push_back(sun);
    }
    // B. LightingManager Lights
    const auto& regLights = LightingManager::getLights();
    for (const auto& pair : regLights) {
        const RegisteredLight& reg = pair.second;
        if (reg.owner && reg.owner->isHidden()) continue;

        FastLight L;
        if (reg.owner) L.pos = reg.config.position + reg.owner->getWorldPosition();
        else L.pos = reg.config.position;

        // Use diffuse color (or color if diffuse is unset)
        float r = (reg.config.diffuse.x < 0) ? reg.config.color.x : reg.config.diffuse.x;
        float g = (reg.config.diffuse.y < 0) ? reg.config.color.y : reg.config.diffuse.y;
        float b = (reg.config.diffuse.z < 0) ? reg.config.color.z : reg.config.diffuse.z;
        L.color = {r, g, b};
        L.constant = reg.config.constant;
        L.linear = reg.config.linear;
        L.quadratic = reg.config.quadratic;
        sceneLights.push_back(L);
    }

    // --- 2. PREPARE OBJECTS ---
    renderQueue.clear();
    const auto& allObjects = ObjectHandler::getAllObjects();
    renderQueue.reserve(allObjects.size());
    for (Object* obj : allObjects) {
        if (!obj || obj->isHidden() || dynamic_cast<Celestial*>(obj)) continue;
        if (dynamic_cast<Cuboid*>(obj) || dynamic_cast<Cube*>(obj)) renderQueue.push_back(obj);
    }
    
    if(primitives.size() != renderQueue.size()) primitives.resize(renderQueue.size());
    if(primitiveIndices.size() != renderQueue.size()) primitiveIndices.resize(renderQueue.size());

    Vec3<float> corners[8] = {{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{-0.5,-0.5,0.5},{0.5,-0.5,0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5}};

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < (int)renderQueue.size(); i++) {
        Object* obj = renderQueue[i];
        Mat4 combinedInv = Mat4::identity();
        Mat4 combinedFwd = Mat4::identity();
        Mat4 combinedRot = Mat4::identity();

        Object* curr = obj;
        while(curr) {
             Mat4 invS = makeInvScale(curr->getScale());
             Mat4 invR = makeRotate(-curr->getRotationAngle(), curr->getRotationAxis()); 
             Mat4 invT = makeInvTranslate(curr->getPosition());
             combinedInv = combinedInv * (invS * (invR * invT)); 

             Mat4 fwdS = makeScale(curr->getScale());
             Mat4 fwdR = makeRotate(curr->getRotationAngle(), curr->getRotationAxis());
             Mat4 fwdT = makeTranslate(curr->getPosition());
             combinedFwd = (fwdT * (fwdR * fwdS)) * combinedFwd; 
             combinedRot = makeRotate(curr->getRotationAngle(), curr->getRotationAxis()) * combinedRot;
             curr = curr->getParent();
        }

        FastObj& fObj = primitives[i];
        fObj.invWorld = combinedInv;
        fObj.worldRot = combinedRot;
        Color3f c = obj->getColor();
        fObj.color = {c.red, c.green, c.blue};
        
        fObj.bounds = AABB();
        for(int k=0; k<8; k++) fObj.bounds.expand(combinedFwd.transformPoint(corners[k]));
        fObj.centroid = fObj.bounds.center();
        primitiveIndices[i] = i;
    }

    if (renderQueue.empty()) return;

    bvhNodes.clear(); bvhNodes.reserve(renderQueue.size() * 2);
    bvhNodes.push_back(BVHNode()); 
    bvhNodes[0].objStart = 0; bvhNodes[0].objCount = renderQueue.size();
    updateNodeBounds(0);
    splitBVH(0);

    // --- 3. RENDER PIXELS ---
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double winX = x * scale + scale/2.0, winY = y * scale + scale/2.0;
            GLdouble nx,ny,nz, fx,fy,fz;
            gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &nx, &ny, &nz);
            gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &fx, &fy, &fz);
            Vec3<float> org(nx,ny,nz), target(fx,fy,fz);
            Vec3<float> dir = target - org;
            float mag = sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
            dir = dir * (1.0f/mag);

            float closestT = 1e9f; int hitIndex = -1; Vec3<float> hitNormal;
            traceBVH(org, dir, closestT, hitIndex, hitNormal, false);

            Vec3<float> finalColor(0.0f, 0.0f, 0.0f); // Start black, add ambient later
            
            if (hitIndex != -1) {
                const FastObj& hitObj = primitives[hitIndex];
                Vec3<float> worldNormal = hitObj.worldRot.transformDir(hitNormal);
                float nm = sqrt(worldNormal.x*worldNormal.x + worldNormal.y*worldNormal.y + worldNormal.z*worldNormal.z);
                if (nm > 0) worldNormal = worldNormal * (1.0f/nm);
                Vec3<float> hitPoint = org + dir * closestT;

                // Base Ambient
                finalColor = hitObj.color * 0.3f; 

                // Loop Lights
                for (const auto& L : sceneLights) {
                    Vec3<float> lightVec = L.pos - hitPoint;
                    float dist2 = lightVec.x*lightVec.x + lightVec.y*lightVec.y + lightVec.z*lightVec.z;
                    float dist = sqrt(dist2);
                    Vec3<float> lightDir = lightVec * (1.0f/dist);

                    float NdotL = worldNormal.dot(lightDir);
                    if (NdotL > 0.0f) {
                        float shadowT = dist; int sIdx = -1; Vec3<float> sNorm;
                        // Offset to prevent acne
                        traceBVH(hitPoint + lightDir * 0.1f, lightDir, shadowT, sIdx, sNorm, true);
                        
                        if (shadowT >= dist - 0.2f) { // Visible
                            float atten = 1.0f / (L.constant + L.linear*dist + L.quadratic*dist2);
                            float diff = NdotL * atten;
                            
                            // Accumulate
                            finalColor.x += hitObj.color.x * L.color.x * diff;
                            finalColor.y += hitObj.color.y * L.color.y * diff;
                            finalColor.z += hitObj.color.z * L.color.z * diff;
                        }
                    }
                }
                // Clamp
                if (finalColor.x > 1.0f) finalColor.x = 1.0f;
                if (finalColor.y > 1.0f) finalColor.y = 1.0f;
                if (finalColor.z > 1.0f) finalColor.z = 1.0f;

            } else {
                finalColor = {0.4f, 0.6f, 0.9f}; // Sky
            }

            int idx = (y * w + x) * 3;
            pixelBuffer[idx]   = (unsigned char)(finalColor.x*255);
            pixelBuffer[idx+1] = (unsigned char)(finalColor.y*255);
            pixelBuffer[idx+2] = (unsigned char)(finalColor.z*255);
        }
    }

    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glRasterPos2i(-1, -1); 
    glPixelZoom((float)scale, (float)scale);
    glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer.data());
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}