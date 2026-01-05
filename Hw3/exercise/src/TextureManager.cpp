#include "TextureManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utilities.h"
#include "bmp.h"

std::map<TextureID, GLuint> TextureManager::textures;

void TextureManager::drawQuadTex(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const Vec3<float>& normal, float uMax, float vMax) {
    
    glBegin(GL_QUADS); 
    glNormal3f(normal.x, normal.y, normal.z);

    //TODO Fix mapping IF wrong IDK
    //! Set texture positions
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(p4.x, p4.y, p4.z);
    
    glTexCoord2f(uMax, 0.0f);
    glVertex3f(p3.x, p3.y, p3.z);
    
    glTexCoord2f(uMax, vMax);
    glVertex3f(p2.x, p2.y, p2.z);
    
    glTexCoord2f(0.0f, vMax);
    glVertex3f(p1.x, p1.y, p1.z);
    
    glEnd();
}

void TextureManager::bind(TextureID id) {
    if (id == TextureID::NONE) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    auto it = textures.find(id);
    if (it == textures.end()) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, it->second);
}

void TextureManager::clear() {
    for (std::pair<const TextureID, GLuint>& pair : textures) {
        glDeleteTextures(1, &pair.second);
    }
    textures.clear();
}

bool TextureManager::init(TextureID id, const std::string& bmpPath) {
    if (id == TextureID::NONE) {
        return false;
    }

    if (textures.count(id)) {
        return true;
    }

    bitmapFileHeader_t fileHeader;
    bitmapInfoHeader_t infoHeader;

    unsigned char* data =
        LoadTextureFile(const_cast<char*>(bmpPath.c_str()), &fileHeader, &infoHeader);

    if (!data) {
        std::cerr << "Failed to load BMP: " << bmpPath << "\n";
        return false;
    }

    if (infoHeader.biBitCount != 24) {
        std::cerr << "Only 24-bit BMP supported: " << bmpPath << "\n";
        free(data);
        return false;
    }

    int width  = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    int byteWidth = width * 3;
    int paddedWidth = byteWidth;
    while (paddedWidth % 4 != 0) paddedWidth++;

    reverseImage(data, byteWidth, height, paddedWidth - byteWidth);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    free(data);

    textures[id] = texID;
    return true;
}
