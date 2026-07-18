#include "edge.h"

template <typename T>
Edge<T>::Edge(Point<T> start, Point<T> end) {
    this->start = start;
    this->end = end;
    this->minY = start.y < end.y ? start.y : end.y;
    this->maxY = start.y > end.y ? start.y : end.y;
    this->currentX = start.y < end.y ? start.x : end.x;
    this->currentColor = start.y < end.y ? start.rgb : end.rgb;
    RGB endColor = start.y < end.y ? end.rgb : start.rgb;
    this->xIncrement = (end.y - start.y) == 0 ? 0 : (float) (end.x - start.x) / (end.y - start.y);

    int colorHeight = maxY - minY;
    this->rIncrement = colorHeight == 0 ? 0 : (float) (endColor.red - currentColor.red) / colorHeight;
    this->gIncrement = colorHeight == 0 ? 0 : (float) (endColor.green - currentColor.green) / colorHeight;
    this->bIncrement = colorHeight == 0 ? 0 : (float) (endColor.blue - currentColor.blue) / colorHeight;
}

template <typename T>
Point<T> Edge<T>::getStart() const {
    return this->start;
}

template <typename T>
Point<T> Edge<T>::getEnd() const {
    return this->end;
}

template <typename T>
T Edge<T>::getMinY() const {
    return this->minY;
}

template <typename T>
T Edge<T>::getMaxY() const {
    return this->maxY;
}

template <typename T>
float Edge<T>::getCurrentX() const {
    return this->currentX;
}

template <typename T>
RGB Edge<T>::getCurrentColor() const {
    return this->currentColor;
}

template <typename T>
void Edge<T>::incrementX() {
    this->currentX += this->xIncrement;
}

template <typename T>
void Edge<T>::incrementColor() {
    this->currentColor.red += this->rIncrement;
    this->currentColor.green += this->gIncrement;
    this->currentColor.blue += this->bIncrement;
}

template class Edge<int>;
template class Edge<float>;