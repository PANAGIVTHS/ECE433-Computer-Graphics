#ifndef HOUSE_H
#define HOUSE_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class House {
private:
    /* data */
public:
    House(/* args */);

    void init();
    void spawn();
};

#endif