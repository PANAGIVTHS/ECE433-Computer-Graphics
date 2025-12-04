#ifndef CLIPPING_WINDOW_H
#define CLIPPING_WINDOW_H
#include "utilities.h"
#include "edge.h"
#include "polygon.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

template <typename T>
class ClippingWindow {
private:
    Point<int> start, end;
    int getMaxX() const;
    int getMaxY() const;
    int getMinX() const;
    int getMinY() const;
    Point<float> intersectEdge(WindowEdge boundary, Edge<float> edge);
public:
    ClippingWindow(Point<int> start, Point<int> end);
    EdgeState getState(WindowEdge boundary, Edge<float> edge);
    void setEnd(Point<int> end);
    void draw() const;
    void clipSelection();
};

#endif