#ifndef TEXTURE_ENUMS_H
#define TEXTURE_ENUMS_H

enum class TextureID {
    NONE = 0,
    GRASS,
    LEAVES,
    IRON,
    WOOD,
    WINDOW,
    MYCELIUM
};

enum class TextureMode {
    STRETCH,
    REPEAT_FIT,
    REPEAT_CUSTOM
};

struct TextureConfig {
    TextureMode mode;
    float uMax = 1.0f;
    float vMax = 1.0f;

    TextureConfig(TextureMode m = TextureMode::STRETCH) 
        : mode(m) {}

    TextureConfig(TextureMode m, float u, float v) 
        : mode(m), uMax(u), vMax(v) {}
};
#endif
