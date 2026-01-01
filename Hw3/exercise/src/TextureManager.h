#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <map>
#include <string>
#include "TextureID.h"
#include "bmp.h"

class TextureManager {
private:
    static std::map<TextureID, GLuint> textures;
public:
    static void bind(TextureID id);
    static bool init(TextureID id, const std::string& bmpPath);
    static void clear();
};

#endif