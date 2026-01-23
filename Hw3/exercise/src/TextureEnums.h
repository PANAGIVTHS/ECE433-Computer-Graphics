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
    X(BONE_SIDE)        \
    X(GRAY_WOOL)        \
    X(BLACK_WOOL)       \
    X(BLUE_WOOL)        \
    X(RED_WOOL)        \
    X(BROWN_WOOL)       \
    X(GREEN_WOOL)       \
    X(MAGENTA_WOOL)     \
    X(ORANGE_WOOL)      \
    X(WHITE_WOOL)       \
    X(YELLOW_WOOL)      \
    X(BROWN_CONCRETE)   \
    X(SPRUCE_PLANKS)    \


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
