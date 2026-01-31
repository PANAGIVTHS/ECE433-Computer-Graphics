#include "RayTracer.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <unordered_set>
#include "Celestial.h" 
#include "LightingManager.h"

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
    AABB bounds;        
    Vec3<float> centroid; 
    Object* original; // Pointer to check if static status changed
};

struct FastLight {
    Vec3<float> pos;
    Vec3<float> color;
    float constant, linear, quadratic;
    bool isSpot;
    Vec3<float> direction;
    float cutOffCos;       
    float spotExp;
};

struct BVHNode {
    AABB box;
    int leftIndex = -1, rightIndex = -1;
    int objStart = 0, objCount = 0;
};

// --- DATA STORAGE ---
// We now separate Static (Walls) from Dynamic (Doors/Player)
std::vector<FastObj> staticPrims;
std::vector<int> staticIndices;
std::vector<BVHNode> staticNodes;
bool staticBuilt = false; // Flag to build only once

std::vector<FastObj> dynamicPrims;
std::vector<int> dynamicIndices;
std::vector<BVHNode> dynamicNodes;

std::vector<FastLight> sceneLights;

// ==========================================
//           BVH BUILDER (Generic)
// ==========================================

void updateNodeBounds(std::vector<BVHNode>& nodes, const std::vector<FastObj>& prims, const std::vector<int>& indices, int nodeIdx) {
    BVHNode& node = nodes[nodeIdx];
    node.box = AABB();
    for (int i = 0; i < node.objCount; ++i) {
        int objIdx = indices[node.objStart + i];
        const AABB& objBox = prims[objIdx].bounds;
        node.box.expand(objBox.min);
        node.box.expand(objBox.max);
    }
}

void splitBVH(std::vector<BVHNode>& nodes, const std::vector<FastObj>& prims, std::vector<int>& indices, int nodeIdx) {
    if (nodes[nodeIdx].objCount <= 2) return; 

    const BVHNode& node = nodes[nodeIdx];
    Vec3<float> extent = node.box.max - node.box.min;
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent.y && extent.z > extent.x) axis = 2;

    float splitPos = (axis == 0) ? node.box.center().x : (axis == 1) ? node.box.center().y : node.box.center().z;

    int i = node.objStart;
    int j = node.objStart + node.objCount - 1;
    while (i <= j) {
        float pos = (axis == 0) ? prims[indices[i]].centroid.x :
                    (axis == 1) ? prims[indices[i]].centroid.y :
                                  prims[indices[i]].centroid.z;
        if (pos < splitPos) i++;
        else {
            std::swap(indices[i], indices[j]);
            j--;
        }
    }

    int leftCount = i - node.objStart;
    int currentObjCount = nodes[nodeIdx].objCount;
    if (leftCount == 0 || leftCount == currentObjCount) return; 

    int rightCount = currentObjCount - leftCount;
    int currentStart = nodes[nodeIdx].objStart;

    int leftChildIdx = nodes.size();
    nodes.push_back(BVHNode());
    int rightChildIdx = nodes.size();
    nodes.push_back(BVHNode());

    nodes[nodeIdx].leftIndex = leftChildIdx;
    nodes[nodeIdx].rightIndex = rightChildIdx;
    nodes[nodeIdx].objCount = 0; 

    nodes[leftChildIdx].objStart = currentStart;
    nodes[leftChildIdx].objCount = leftCount;
    nodes[rightChildIdx].objStart = i; 
    nodes[rightChildIdx].objCount = rightCount; 

    updateNodeBounds(nodes, prims, indices, leftChildIdx);
    updateNodeBounds(nodes, prims, indices, rightChildIdx);
    splitBVH(nodes, prims, indices, leftChildIdx);
    splitBVH(nodes, prims, indices, rightChildIdx);
}

// ==========================================
//           INTERSECTION LOGIC
// ==========================================

// Optimization: Inline and reduce divisions
inline bool intersectUnitBox(const Vec3<float>& origin, const Vec3<float>& dir, float& tOut) {
    // Standard slab method without normal calculation (faster)
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
        return true;
    }
    return false;
}

// Separate function to calculate normal ONLY when needed
Vec3<float> getBoxNormal(const Vec3<float>& p) {
    Vec3<float> minB(-0.5f, -0.5f, -0.5f), maxB(0.5f, 0.5f, 0.5f);
    float eps = 1e-3f;
    if (std::abs(p.x - minB.x) < eps) return {-1, 0, 0};
    if (std::abs(p.x - maxB.x) < eps) return {1, 0, 0};
    if (std::abs(p.y - minB.y) < eps) return {0, -1, 0};
    if (std::abs(p.y - maxB.y) < eps) return {0, 1, 0};
    if (std::abs(p.z - minB.z) < eps) return {0, 0, -1};
    return {0, 0, 1};
}

void traceTree(const std::vector<BVHNode>& nodes, const std::vector<FastObj>& prims, const std::vector<int>& indices, 
               const Vec3<float>& rayOrg, const Vec3<float>& rayDir, float& closestT, int& hitIndex, bool& hitStatic, bool isStaticTree, bool shadowMode) {
    if(nodes.empty()) return;

    int stack[64]; int stackPtr = 0; stack[stackPtr++] = 0; 
    Vec3<float> invDir = {1.0f/rayDir.x, 1.0f/rayDir.y, 1.0f/rayDir.z};

    while (stackPtr > 0) {
        int nodeIdx = stack[--stackPtr];
        const BVHNode& node = nodes[nodeIdx];
        if (node.box.intersect(rayOrg, invDir) >= closestT) continue;

        if (node.leftIndex == -1) { 
            for (int i = 0; i < node.objCount; ++i) {
                int objIdx = indices[node.objStart + i];
                const FastObj& obj = prims[objIdx];
                Vec3<float> lOrg = obj.invWorld.transformPoint(rayOrg);
                Vec3<float> lDir = obj.invWorld.transformDir(rayDir);
                float t = 0;
                // Optimization: Don't calc normal here
                if (intersectUnitBox(lOrg, lDir, t)) {
                    if (t < closestT && t > 0.001f) {
                        if (shadowMode) { closestT = t; return; }
                        closestT = t; 
                        hitIndex = objIdx; 
                        hitStatic = isStaticTree;
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

    int scale = 5; 
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

    // --- 1. COLLECT LIGHTS & BLOCKERS ---
    sceneLights.clear();
    std::unordered_set<Object*> lightOwners; 

    if (GameManager::getSun()) {
        FastLight sun;
        sun.pos = GameManager::getSun()->getWorldPosition();
        sun.color = {1.0f, 1.0f, 1.0f}; 
        sun.constant = 1.0f; sun.linear = 0.0f; sun.quadratic = 0.0f;
        sun.isSpot = false;
        sceneLights.push_back(sun);
    }
    const auto& regLights = LightingManager::getLights();
    for (const auto& pair : regLights) {
        const RegisteredLight& reg = pair.second;
        if (reg.owner && reg.owner->isHidden()) continue;
        if (reg.owner) lightOwners.insert(reg.owner); 

        FastLight L;
        float r = (reg.config.diffuse.x < 0) ? reg.config.color.x : reg.config.diffuse.x;
        float g = (reg.config.diffuse.y < 0) ? reg.config.color.y : reg.config.diffuse.y;
        float b = (reg.config.diffuse.z < 0) ? reg.config.color.z : reg.config.diffuse.z;
        L.color = {r, g, b};
        L.constant = reg.config.constant;
        L.linear = reg.config.linear;
        L.quadratic = reg.config.quadratic;
        L.isSpot = (reg.config.spotCutoff < 179.0f);
        L.spotExp = reg.config.spotExponent;
        L.cutOffCos = cos(reg.config.spotCutoff * M_PI / 180.0f);
        
        if (reg.owner) {
            L.pos = reg.config.position + reg.owner->getWorldPosition();
            Mat4 rotMat = Mat4::identity();
            Object* curr = reg.owner;
            std::vector<Object*> chain;
            while(curr) { chain.push_back(curr); curr = curr->getParent(); }
            for(int k=chain.size()-1; k>=0; k--) {
                Object* o = chain[k];
                rotMat = makeRotate(o->getRotationAngle(), o->getRotationAxis()) * rotMat;
            }
            L.direction = rotMat.transformDir(reg.config.spotDirection);
        } else {
            L.pos = reg.config.position;
            L.direction = reg.config.spotDirection; 
        }
        float dMag = sqrt(L.direction.x*L.direction.x + L.direction.y*L.direction.y + L.direction.z*L.direction.z);
        if(dMag > 0) L.direction = L.direction * (1.0f/dMag);
        sceneLights.push_back(L);
    }

    // --- 2. BUILD SCENE (STATIC vs DYNAMIC) ---
    // Clear dynamic every frame
    dynamicPrims.clear();
    dynamicIndices.clear();
    dynamicNodes.clear();

    const auto& allObjects = ObjectHandler::getAllObjects();
    
    // Helper to process an object into a FastObj
    auto processObj = [&](Object* obj) -> FastObj {
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
        FastObj fObj;
        fObj.invWorld = combinedInv;
        fObj.worldRot = combinedRot;
        fObj.original = obj;
        fObj.bounds = AABB();
        Vec3<float> corners[8] = {{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{-0.5,-0.5,0.5},{0.5,-0.5,0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5}};
        for(int k=0; k<8; k++) fObj.bounds.expand(combinedFwd.transformPoint(corners[k]));
        fObj.centroid = fObj.bounds.center();
        return fObj;
    };

    // If static tree not built, build it now
    if (!staticBuilt) {
        staticPrims.clear();
        staticIndices.clear();
        staticNodes.clear();
        for (Object* obj : allObjects) {
            if (!obj || obj->isHidden() || dynamic_cast<Celestial*>(obj)) continue;
            // Skip light owners
            bool isLight = false; Object* p=obj; while(p){if(lightOwners.count(p)){isLight=true;break;}p=p->getParent();}
            if(isLight) continue;

            if ((dynamic_cast<Cuboid*>(obj) || dynamic_cast<Cube*>(obj)) && obj->isStatic()) {
                staticPrims.push_back(processObj(obj));
                staticIndices.push_back(staticPrims.size()-1);
            }
        }
        if (!staticPrims.empty()) {
            staticNodes.reserve(staticPrims.size()*2);
            staticNodes.push_back(BVHNode());
            staticNodes[0].objCount = staticPrims.size();
            updateNodeBounds(staticNodes, staticPrims, staticIndices, 0);
            splitBVH(staticNodes, staticPrims, staticIndices, 0);
        }
        staticBuilt = true; 
    }

    // Build Dynamic Tree (Every Frame)
    for (Object* obj : allObjects) {
        if (!obj || obj->isHidden() || dynamic_cast<Celestial*>(obj)) continue;
        bool isLight = false; Object* p=obj; while(p){if(lightOwners.count(p)){isLight=true;break;}p=p->getParent();}
        if(isLight) continue;

        if ((dynamic_cast<Cuboid*>(obj) || dynamic_cast<Cube*>(obj)) && !obj->isStatic()) {
            dynamicPrims.push_back(processObj(obj));
            dynamicIndices.push_back(dynamicPrims.size()-1);
        }
    }
    if (!dynamicPrims.empty()) {
        dynamicNodes.reserve(dynamicPrims.size()*2);
        dynamicNodes.push_back(BVHNode());
        dynamicNodes[0].objCount = dynamicPrims.size();
        updateNodeBounds(dynamicNodes, dynamicPrims, dynamicIndices, 0);
        splitBVH(dynamicNodes, dynamicPrims, dynamicIndices, 0);
    }

    // --- 3. RENDER MASK ---
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

            float closestT = 1e9f; int hitIndex = -1; bool hitStatic = false;
            
            // Check Both Trees
            traceTree(staticNodes, staticPrims, staticIndices, org, dir, closestT, hitIndex, hitStatic, true, false);
            traceTree(dynamicNodes, dynamicPrims, dynamicIndices, org, dir, closestT, hitIndex, hitStatic, false, false);

            Vec3<float> maskValue(1.0f, 1.0f, 1.0f); // Default White (Sky)
            
            if (hitIndex != -1) {
                // Fetch Hit Object Data
                const FastObj& hitObj = hitStatic ? staticPrims[hitIndex] : dynamicPrims[hitIndex];
                
                // Calculate Normal (Deferred)
                Vec3<float> lOrg = hitObj.invWorld.transformPoint(org);
                Vec3<float> lDir = hitObj.invWorld.transformDir(dir);
                float tLocal = 0; intersectUnitBox(lOrg, lDir, tLocal); // Re-calc tlocal
                Vec3<float> localNormal = getBoxNormal(lOrg + lDir * tLocal);
                Vec3<float> worldNormal = hitObj.worldRot.transformDir(localNormal);
                
                float nm = sqrt(worldNormal.x*worldNormal.x + worldNormal.y*worldNormal.y + worldNormal.z*worldNormal.z);
                if (nm > 0) worldNormal = worldNormal * (1.0f/nm);
                
                Vec3<float> hitPoint = org + dir * closestT;

                maskValue = {0.3f, 0.3f, 0.3f}; // Ambient

                for (const auto& L : sceneLights) {
                    Vec3<float> safePos = L.pos;
                    if(L.isSpot) safePos = safePos + L.direction * 1.5f; 

                    Vec3<float> lightVec = safePos - hitPoint;
                    float dist2 = lightVec.x*lightVec.x + lightVec.y*lightVec.y + lightVec.z*lightVec.z;
                    float dist = sqrt(dist2);
                    Vec3<float> lightDir = lightVec * (1.0f/dist);

                    float spotEffect = 1.0f;
                    if (L.isSpot) {
                        float theta = L.direction.dot(lightDir * -1.0f); 
                        if (theta > L.cutOffCos) spotEffect = pow(theta, L.spotExp); 
                        else spotEffect = 0.0f; 
                    }
                    if (spotEffect <= 0.001f) continue;

                    float NdotL = worldNormal.dot(lightDir);
                    if (NdotL > 0.0f) {
                        float shadowT = dist; int sIdx = -1; bool sStatic=false;
                        // Shadow Check Both Trees
                        traceTree(staticNodes, staticPrims, staticIndices, hitPoint + lightDir * 0.1f, lightDir, shadowT, sIdx, sStatic, true, true);
                        if(shadowT >= dist - 0.2f) { // If didn't hit static closer than light
                             traceTree(dynamicNodes, dynamicPrims, dynamicIndices, hitPoint + lightDir * 0.1f, lightDir, shadowT, sIdx, sStatic, false, true);
                        }
                        
                        if (shadowT >= dist - 0.2f) { 
                            float atten = 1.0f / (L.constant + L.linear*dist + L.quadratic*dist2);
                            float intensity = NdotL * atten * spotEffect * 5.0f; 
                            maskValue.x += L.color.x * intensity;
                            maskValue.y += L.color.y * intensity;
                            maskValue.z += L.color.z * intensity;
                        }
                    }
                }
                if (maskValue.x > 1.0f) maskValue.x = 1.0f;
                if (maskValue.y > 1.0f) maskValue.y = 1.0f;
                if (maskValue.z > 1.0f) maskValue.z = 1.0f;
            }

            int idx = (y * w + x) * 3;
            pixelBuffer[idx]   = (unsigned char)(maskValue.x*255);
            pixelBuffer[idx+1] = (unsigned char)(maskValue.y*255);
            pixelBuffer[idx+2] = (unsigned char)(maskValue.z*255);
        }
    }

    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glRasterPos2i(-1, -1); 
    glPixelZoom((float)scale, (float)scale);
    glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer.data());
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}