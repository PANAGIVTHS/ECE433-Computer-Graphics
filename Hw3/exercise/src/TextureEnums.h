#ifndef TEXTURE_ENUMS_H
#define TEXTURE_ENUMS_H

#define TEXTURE_LIST(X) \
    X(GRASS)            \
    X(LEAVES)           \
    X(PORTAL)           \
    X(OBSIDIAN)         \
    X(IRON)             \
    X(WOOD)             \
    X(WINDOW)           \
    X(MYCELIUM)         \
    X(STONE)            \
    X(GRAY_WOOL)        \
    X(BLACK_WOOL)       \
    X(BONE_SIDE)


enum class TextureID {
#define X(name) name,
    TEXTURE_LIST(X)
#undef X
    NONE
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
