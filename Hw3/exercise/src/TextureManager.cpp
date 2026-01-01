#include "TextureManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "bmp.h"

std::map<TextureID, GLuint> TextureManager::textures;

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
        LoadBitmapFile(const_cast<char*>(bmpPath.c_str()), &fileHeader, &infoHeader);

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
