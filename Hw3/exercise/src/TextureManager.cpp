#include "TextureManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utilities.h"
#include "bmp.h"

std::map<TextureID, GLuint> TextureManager::textures;

void TextureManager::drawQuadTex(const Vec3<GLfloat>& p1, const Vec3<GLfloat>& p2, const Vec3<GLfloat>& p3, const Vec3<GLfloat>& p4, const Vec3<GLfloat>& normal, float uMax, float vMax) {
    
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

bool TextureManager::init(TextureID id, const std::string& path, int width, int height) {
    if (id == TextureID::NONE) return false;
    if (textures.count(id)) return true; // Already loaded

    unsigned char* data = nullptr;
    int imgWidth = 0;
    int imgHeight = 0;
    
    if (width > 0 && height > 0) {
        imgWidth = width;
        imgHeight = height;
        
        data = LoadTextureFile(const_cast<char*>(path.c_str()), width, height);
        
        if (!data) {
            std::cerr << "Failed to load RAW: " << path << "\n";
            return false;
        }
    } 
    else {
        bitmapFileHeader_t fileHeader;
        bitmapInfoHeader_t infoHeader;

        data = LoadTextureFile(const_cast<char*>(path.c_str()), &fileHeader, &infoHeader);

        if (!data) {
            std::cerr << "Failed to load BMP: " << path << "\n";
            return false;
        }

        if (infoHeader.biBitCount != 24) {
            std::cerr << "Only 24-bit BMP supported: " << path << "\n";
            free(data);
            return false;
        }

        imgWidth = infoHeader.biWidth;
        imgHeight = infoHeader.biHeight;

        int byteWidth = imgWidth * 3;
        int paddedWidth = byteWidth;
        while (paddedWidth % 4 != 0) paddedWidth++;
        
        reverseImage(data, byteWidth, imgHeight, paddedWidth - byteWidth);
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (width > 0 && height > 0) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    
    // Generate Mipmaps
    gluBuild2DMipmaps(
        GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, 
        GL_RGB, GL_UNSIGNED_BYTE, data
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    free(data);

    textures[id] = texID;
    return true;
}