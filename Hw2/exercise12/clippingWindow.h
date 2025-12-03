#ifndef CLIPPING_WINDOW_H
#define CLIPPING_WINDOW_H
#include "utilities.h"
#include "edge.h"



class ClippingWindow {
private:
    Point start, end;
    int getMaxX() const;
    int getMaxY() const;
    int getMinX() const;
    int getMinY() const;
    PointFloat ClippingWindow::intersectEdge(WindowEdge boundary, Edge edge);
public:
    ClippingWindow(Point start, Point end);
    EdgeState getState(WindowEdge boundary, Edge edge);
};

#endif