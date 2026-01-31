#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Object.h"
#include "GameManager.h"
#include <vector>

class RayTracer {
public:
    // Main render function. Call this from display()
    static void render(int screenWidth, int screenHeight);

    // Toggle ray tracing on/off
    static void toggle() { enabled = !enabled; }
    static bool isEnabled() { return enabled; }

private:
    static bool enabled;
    static std::vector<unsigned char> pixelBuffer;

    struct Ray {
        Vec3<float> origin;
        Vec3<float> dir;
    };

    struct HitInfo {
        float t;
        Vec3<float> normal;
        Object* object;
    };

    // Core tracing logic
    static Vec3<float> trace(const Ray& ray, const std::vector<Object*>& objects);
    
    // Optimized Unit Cube intersection (for Cube/Cuboid)
    static bool intersectCube(const Ray& localRay, float& t, Vec3<float>& normal);
};

#endif