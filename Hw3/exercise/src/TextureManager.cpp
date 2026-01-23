#include "TextureManager.h"
#include <iostream>
#include "utilities.h"
#include "bmp.h"

std::map<TextureID, GLuint> TextureManager::textures;
std::set<TextureID> TextureManager::transparentTextures;

// Helper function for linear interpolation (Lerp)
static Vec3<float> mix(const Vec3<float>& a, const Vec3<float>& b, float t) {
    return a + (b - a) * t;
}

void TextureManager::drawQuadTex(const Vec3<GLfloat>& p1, const Vec3<GLfloat>& p2, const Vec3<GLfloat>& p3, const Vec3<GLfloat>& p4, const Vec3<GLfloat>& normal, float uMax, float vMax, int subdivisions) {
    
    // Ensure at least 1 subdivision to avoid divide-by-zero or no draw
    if (subdivisions < 1) subdivisions = 1;

    glNormal3f(normal.x, normal.y, normal.z);
    glBegin(GL_QUADS); 

    float step = 1.0f / (float)subdivisions;

    for (int i = 0; i < subdivisions; i++) {
        for (int j = 0; j < subdivisions; j++) {
            // Calculate grid coordinates (0.0 to 1.0)
            float u0 = i * step;
            float u1 = (i + 1) * step;
            float v0 = j * step;
            float v1 = (j + 1) * step;

            // Interpolate Positions
            // p4(BL) -> p3(BR) (Bottom Edge)
            // p1(TL) -> p2(TR) (Top Edge)
            
            Vec3<float> bot0 = mix(p4, p3, u0); 
            Vec3<float> bot1 = mix(p4, p3, u1);
            Vec3<float> top0 = mix(p1, p2, u0);
            Vec3<float> top1 = mix(p1, p2, u1);

            // Final 4 corners of the small sub-quad
            Vec3<float> q_bl = mix(bot0, top0, v0); // Bottom-Left
            Vec3<float> q_br = mix(bot1, top1, v0); // Bottom-Right
            Vec3<float> q_tr = mix(bot1, top1, v1); // Top-Right
            Vec3<float> q_tl = mix(bot0, top0, v1); // Top-Left

            // Texture Coordinates (scaled by uMax/vMax)
            float t_u0 = u0 * uMax;
            float t_u1 = u1 * uMax;
            float t_v0 = v0 * vMax;
            float t_v1 = v1 * vMax;

            // Draw the sub-quad
            // Order matches original: BL -> BR -> TR -> TL
            glTexCoord2f(t_u0, t_v0); glVertex3f(q_bl.x, q_bl.y, q_bl.z);
            glTexCoord2f(t_u1, t_v0); glVertex3f(q_br.x, q_br.y, q_br.z);
            glTexCoord2f(t_u1, t_v1); glVertex3f(q_tr.x, q_tr.y, q_tr.z);
            glTexCoord2f(t_u0, t_v1); glVertex3f(q_tl.x, q_tl.y, q_tl.z);
        }
    }

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
    init(TextureID::WINDOW, "../Texture_Images/ss0052.bmp", "../Texture_Images/ss0052_mask.bmp");
    init(TextureID::LEAVES, "../Texture_Images/azalea_top.bmp");
    init(TextureID::MYCELIUM, "../Texture_Images/mycelium_top.bmp");
    init(TextureID::STONE, "../Texture_Images/cobblestone.bmp");
    init(TextureID::GRAY_WOOL, "../Texture_Images/gray_wool.bmp");
    init(TextureID::BONE_SIDE, "../Texture_Images/bone_block_side.bmp");
    init(TextureID::BLACK_WOOL, "../Texture_Images/black_wool.bmp");
}

//TODO Do No magic color
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
    free(rgbaData);

    textures[id] = texID;
    return true;
}

bool TextureManager::isTransparent(TextureID id) {
    return transparentTextures.find(id) != transparentTextures.end();
}

bool TextureManager::init(TextureID id, const std::string& bmpPath, const std::string& maskPath) {
    if (id == TextureID::NONE) return false;
    if (textures.count(id)) return true;

    bitmapFileHeader_t fHeader1, fHeader2;
    bitmapInfoHeader_t iHeader1, iHeader2;

    unsigned char* colorData = LoadTextureFile(const_cast<char*>(bmpPath.c_str()), &fHeader1, &iHeader1);
    if (!colorData) {
        std::cerr << "Failed to load Color Map: " << bmpPath << "\n";
        return false;
    }

    unsigned char* maskData = LoadTextureFile(const_cast<char*>(maskPath.c_str()), &fHeader2, &iHeader2);
    if (!maskData) {
        std::cerr << "Failed to load Mask Map: " << maskPath << "\n";
        free(colorData);
        return false;
    }

    if (iHeader1.biWidth != iHeader2.biWidth || iHeader1.biHeight != iHeader2.biHeight) {
        std::cerr << "Texture dimensions do not match for: " << bmpPath << "\n";
        free(colorData);
        free(maskData);
        return false;
    }

    int width = iHeader1.biWidth;
    int height = iHeader1.biHeight;

    unsigned char* finalData = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned char));

    unsigned char* srcColor = colorData;
    unsigned char* srcMask = maskData;
    unsigned char* dst = finalData;

    for (int i = 0; i < width * height; i++) {
        unsigned char b = srcColor[0];
        unsigned char g = srcColor[1];
        unsigned char r = srcColor[2];

        unsigned char alpha = srcMask[2]; 

        dst[0] = b;
        dst[1] = g;
        dst[2] = r;
        dst[3] = alpha;

        srcColor += 3;
        srcMask += 3;
        dst += 4;
    }

    free(colorData);
    free(maskData);

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

    gluBuild2DMipmaps(
        GL_TEXTURE_2D, GL_RGBA, width, height, 
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, finalData
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    free(finalData);

    transparentTextures.insert(id);
    
    textures[id] = texID;
    return true;
}