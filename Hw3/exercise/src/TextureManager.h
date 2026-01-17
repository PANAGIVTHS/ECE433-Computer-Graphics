#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <map>
#include <string>
#include "TextureEnums.h"
#include "utilities.h"
#include "bmp.h"

class TextureManager {
private:
    static std::map<TextureID, GLuint> textures;
public:
    static void drawQuadTex(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4,
                            const Vec3<float>& normal, float uMax, float vMax);
    static void bind(TextureID id);
    static bool init(TextureID id, const std::string& bmpPath, int width = 0, int height = 0);
    static bool init(TextureID id, const std::string& bmpPath, const std::string& maskPath);
    static void clear();

    static void init();
};

#endif