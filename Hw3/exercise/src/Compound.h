#ifndef HOUSE_H
#define HOUSE_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Compound {
private:
    /* data */
public:
    Compound(/* args */);

    void init();
    void spawn();
};

#endif