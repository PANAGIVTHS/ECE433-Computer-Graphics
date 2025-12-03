#include "edge.h"

Edge::Edge(Point start, Point end) {
    this->start = start;
    this->end = end;
    this->minY = start.y < end.y ? start.y : end.y;
    this->maxY = start.y > end.y ? start.y : end.y;
    this->currentX = start.y < end.y ? start.x : end.x;
    this->currentColor = start.rgb;
    this->xIncrement = (end.y - start.y) == 0 ? 0 : (float) (end.x - start.x) / (end.y - start.y);
    this->rIncrement = 0;
    this->gIncrement = 0;
    this->bIncrement = 0;
}

Point Edge::getStart() const {
    return this->start;
}

Point Edge::getEnd() const {
    return this->end;
}

int Edge::getMinY() const {
    return this->minY;
}

int Edge::getMaxY() const {
    return this->maxY;
}

float Edge::getCurrentX() const {
    return this->currentX;
}

RGB Edge::getCurrentColor() const {
    return this->currentColor;
}

void Edge::incrementX() {
    this->currentX += this->xIncrement;
}

void Edge::incrementColor() {
    this->currentColor.red += this->rIncrement;
    this->currentColor.green += this->gIncrement;
    this->currentColor.blue += this->bIncrement;
}

void Edge::setHeight(int height) {
    this->rIncrement = (float) (end.rgb.red - start.rgb.red) / height;
    this->gIncrement = (float) (end.rgb.green - start.rgb.green) / height;
    this->bIncrement = (float) (end.rgb.blue - start.rgb.blue) / height;
}