#ifndef EDGE_H
#define EDGE_H
#include "utilities.h"

class Edge
{
private:
    Point start, end;
    int minY, maxY;
    float currentX, xIncrement;
    RGB currentColor;
    float rIncrement, gIncrement, bIncrement;
public:
    Edge(Point start, Point end);

    int getMinY() const;
    int getMaxY() const;
    Point getStart() const;
    Point getEnd() const;
    float getCurrentX() const;
    RGB getCurrentColor() const;
    void incrementX();
    void incrementColor();
    void setHeight(int height);
};

#endif