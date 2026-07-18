#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "TextureEnums.h"
#include <map>


#define MATERIAL_LIST(X) \
    X(MATTE)             \
    X(SHINY)             \
    X(COLD_LIGHT)             \

enum class MaterialID {
#define X(name) name,
    MATERIAL_LIST(X)
#undef X
    NONE
};

struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;
};

class MaterialManager {
private:

public:
    static inline std::map<MaterialID, Material> materials;
    static void init(); 
    static void bind(MaterialID id);
};
#endif