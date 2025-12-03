#include "clippingWindow.h"

ClippingWindow::ClippingWindow(Point start, Point end) {
    this->start = start;
    this->end = end;
}

void ClippingWindow::setEnd(Point end) {
    this->end = end;
}

void ClippingWindow::draw() const {
    glRecti(start.x, start.y, end.x, end.y);
}

int ClippingWindow::getMinY() const {
    return start.y < end.y ? start.y : end.y;
}

int ClippingWindow::getMaxY() const {
    return start.y > end.y ? start.y : end.y;
}

int ClippingWindow::getMinX() const {
    return start.x < end.x ? start.x : end.x;
}

int ClippingWindow::getMaxX() const {
    return start.x > end.x ? start.x : end.x;
}

EdgeState ClippingWindow::getState(WindowEdge boundary, Edge edge) {
    auto classifyState = [](bool s_in, bool e_in) {
        return s_in ? (e_in ? IN_IN : IN_OUT)
                    : (e_in ? OUT_IN : OUT_OUT);
    };

    switch (boundary) {
        case LEFT: {
            return classifyState(edge.getStart().x >= getMinX(), edge.getEnd().x >= getMinX());
            break;
        }
        case RIGHT: {
            return classifyState(edge.getStart().x <= getMaxX(), edge.getEnd().x <= getMaxX());
            break;
        }
        case BOT: {
            return classifyState(edge.getStart().y >= getMinY(), edge.getEnd().y >= getMinY());
            break;
        }
        case TOP: {
            return classifyState(edge.getStart().y <= getMaxY(), edge.getEnd().y <= getMaxY());
            break;
        }
    }
}

PointFloat ClippingWindow::intersectEdge(WindowEdge boundary, Edge edge) {
    PointFloat intersectPoint;
    intersectPoint.rgb = edge.getCurrentColor(); // TODO: increment color

    Point start = edge.getStart();
    Point end = edge.getEnd();

    float slope = 0.0f;
    bool vertical = (start.x == end.x);

    if (!vertical) {
        slope = (float)(end.y - start.y) / (float)(end.x - start.x);
    }

    switch (boundary) {
        case LEFT: {
            intersectPoint.x = (float)getMinX();
            intersectPoint.y = vertical
                ? (float)start.y
                : (float)start.y + slope * (intersectPoint.x - (float)start.x);
            break;
        }
        case RIGHT: {
            intersectPoint.x = (float)getMaxX();
            intersectPoint.y = vertical
                ? (float)start.y
                : (float)start.y + slope * (intersectPoint.x - (float)start.x);
            break;
        }
        case BOT: {
            intersectPoint.y = (float)getMinY();
            intersectPoint.x = (slope == 0.0f)
                ? (float)start.x
                : (float)start.x + ((intersectPoint.y - (float)start.y) / slope);
            break;
        }
        case TOP: {
            intersectPoint.y = (float)getMaxY();
            intersectPoint.x = (slope == 0.0f)
                ? (float)start.x
                : (float)start.x + ((intersectPoint.y - (float)start.y) / slope);
            break;
        }
    }

    return intersectPoint;
}

void ClippingWindow::ClipSelection() {
    // for all polygons
    PointFloat oldPoints = toFloat();
}