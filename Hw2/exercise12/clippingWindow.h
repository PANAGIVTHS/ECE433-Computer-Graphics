#ifndef CLIPPING_WINDOW_H
#define CLIPPING_WINDOW_H
#include "utilities.h"
#include "edge.h"



class ClippingWindow {
private:
    Point start, end;
public:
    ClippingWindow(Point start, Point end);
    static EdgeState getState(WindowEdge boundary, Edge edge);
};

#endif