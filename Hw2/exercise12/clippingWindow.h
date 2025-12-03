#ifndef CLIPPING_WINDOW_H
#define CLIPPING_WINDOW_H
#include "utilities.h"
#include "edge.h"
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class ClippingWindow {
private:
    Point start, end;
    int getMaxX() const;
    int getMaxY() const;
    int getMinX() const;
    int getMinY() const;
    PointFloat ClippingWindow::intersectEdge(WindowEdge boundary, Edge edge);
    void ClippingWindow::ClipPolygon();
public:
    ClippingWindow(Point start, Point end);
    void ClippingWindow::ClipSelection();
    EdgeState getState(WindowEdge boundary, Edge edge);
    void setEnd(Point end);
    void draw() const;
};

#endif