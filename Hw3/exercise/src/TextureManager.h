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
    static void clear();

    static void init() {
        glEnable(GL_TEXTURE_2D);
        
        TextureManager::init(TextureID::IRON, "../Texture_Images/LaGioconda.bmp");
        TextureManager::init(TextureID::GRASS, "../Texture_Images/grass.raw", 256, 256);
        TextureManager::init(TextureID::WOOD, "../Texture_Images/oak_log.bmp");
        TextureManager::init(TextureID::WINDOW, "../Texture_Images/ss0052.bmp");
        TextureManager::init(TextureID::LEAVES, "../Texture_Images/azalea_top.bmp");
        TextureManager::init(TextureID::MYCELIUM, "../Texture_Images/cobblestone.bmp");
    }
};

#endif