#ifndef EDGE_H
#define EDGE_H
#include "utilities.h"

template <typename T>
class Edge {
private:
    Point<T> start, end;
    T minY, maxY;
    float currentX, xIncrement;
    RGB currentColor;
    float rIncrement, gIncrement, bIncrement;
public:
    Edge(Point<T> start, Point<T> end);

    T getMinY() const;
    T getMaxY() const;
    Point<T> getStart() const;
    Point<T> getEnd() const;
    float getCurrentX() const;
    RGB getCurrentColor() const;
    void incrementX();
    void incrementColor();
};

#endif