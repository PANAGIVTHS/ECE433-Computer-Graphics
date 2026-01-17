#include "TextureManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utilities.h"
#include "bmp.h"

// TODO Pipelining to ensure objects with transparent textures are drawn last.

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

// This function enables transparency globally
void TextureManager::init() {
    glEnable(GL_TEXTURE_2D);
    
    // ENABLE BLENDING FOR TRANSPARENCY
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load textures
    init(TextureID::IRON, "../Texture_Images/LaGioconda.bmp", "../Texture_Images/LaGioconda_mask.bmp");
    init(TextureID::GRASS, "../Texture_Images/grass.raw", 256, 256);
    init(TextureID::WOOD, "../Texture_Images/oak_log.bmp");
    init(TextureID::WINDOW, "../Texture_Images/ss0052.bmp");
    init(TextureID::LEAVES, "../Texture_Images/azalea_top.bmp");
    init(TextureID::MYCELIUM, "../Texture_Images/diamond_block.bmp");
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

    // RGBA conversion add alpha channel
    // Allocate a new buffer with 4 components (BGRA) instead of 3 (BGR)
    unsigned char* rgbaData = (unsigned char*)malloc(imgWidth * imgHeight * 4 * sizeof(unsigned char));
    if (!rgbaData) {
        std::cerr << "Out of memory for texture RGBA conversion.\n";
        free(data);
        return false;
    }

    unsigned char* src = data;
    unsigned char* dst = rgbaData;
    int numPixels = imgWidth * imgHeight;

    for (int i = 0; i < numPixels; i++) {
        unsigned char b = src[0];
        unsigned char g = src[1];
        unsigned char r = src[2];

        dst[0] = b;
        dst[1] = g;
        dst[2] = r;

        // If color is Magenta (255, 0, 255), make it transparent.
        if (r == 255 && g == 0 && b == 255) {
            dst[3] = 200; 
        } else {
            dst[3] = 255;
        }

        // Move to next
        src += 3;
        dst += 4;
    }

    // Free the original RGB data
    free(data);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 

    // Build Mipmaps using the new RGBA data
    gluBuild2DMipmaps(
        GL_TEXTURE_2D, GL_RGBA, imgWidth, imgHeight, 
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, rgbaData
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    free(rgbaData); // Free the RGBA buffer

    textures[id] = texID;
    return true;
}

bool TextureManager::init(TextureID id, const std::string& bmpPath, const std::string& maskPath) {
    if (id == TextureID::NONE) return false;
    if (textures.count(id)) return true;

    bitmapFileHeader_t fHeader1, fHeader2;
    bitmapInfoHeader_t iHeader1, iHeader2;

    // 1. Load the Color Map (The actual image)
    unsigned char* colorData = LoadTextureFile(const_cast<char*>(bmpPath.c_str()), &fHeader1, &iHeader1);
    if (!colorData) {
        std::cerr << "Failed to load Color Map: " << bmpPath << "\n";
        return false;
    }

    // 2. Load the Alpha Mask (The grayscale transparency info)
    unsigned char* maskData = LoadTextureFile(const_cast<char*>(maskPath.c_str()), &fHeader2, &iHeader2);
    if (!maskData) {
        std::cerr << "Failed to load Mask Map: " << maskPath << "\n";
        free(colorData);
        return false;
    }

    // 3. Ensure dimensions match
    if (iHeader1.biWidth != iHeader2.biWidth || iHeader1.biHeight != iHeader2.biHeight) {
        std::cerr << "Texture dimensions do not match for: " << bmpPath << "\n";
        free(colorData);
        free(maskData);
        return false;
    }

    int width = iHeader1.biWidth;
    int height = iHeader1.biHeight;

    // 4. Allocate memory for RGBA (4 channels)
    unsigned char* finalData = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned char));
    
    // 5. Merge them!
    unsigned char* srcColor = colorData;
    unsigned char* srcMask = maskData;
    unsigned char* dst = finalData;

    for (int i = 0; i < width * height; i++) {
        // --- Color from File 1 ---
        // BMP is stored as BGR, so we flip to RGB for OpenGL if we want, 
        // or keep BGR and use GL_BGRA_EXT. Let's stick to BGR -> BGRA for simplicity.
        unsigned char b = srcColor[0];
        unsigned char g = srcColor[1];
        unsigned char r = srcColor[2];

        // --- Opacity from File 2 ---
        // We read the red channel of the mask (since it's grayscale, r=g=b)
        // If mask pixel is Black (0), Alpha becomes 0 (Transparent)
        // If mask pixel is White (255), Alpha becomes 255 (Opaque)
        // If mask pixel is Gray (128), Alpha becomes 128 (See-through glass)
        unsigned char alpha = srcMask[2]; 

        dst[0] = b;
        dst[1] = g;
        dst[2] = r;
        dst[3] = alpha; // The magic happens here

        srcColor += 3;
        srcMask += 3;
        dst += 4;
    }

    // Clean up the raw inputs
    free(colorData);
    free(maskData);

    // 6. Upload to OpenGL
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 

    // Build Mipmaps with Alpha
    gluBuild2DMipmaps(
        GL_TEXTURE_2D, GL_RGBA, width, height, 
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, finalData
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    free(finalData);

    textures[id] = texID;
    return true;
}