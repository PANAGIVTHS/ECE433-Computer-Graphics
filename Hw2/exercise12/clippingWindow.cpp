#include "clippingWindow.h"

ClippingWindow::ClippingWindow(Point<int> start, Point<int> end) {
    this->start = start;
    this->end = end;
}

void ClippingWindow::setEnd(Point<int> end) {
    this->end = end;
}

void ClippingWindow::draw() const {
    glRecti(start.x, start.y, end.x, end.y);
}

bool ClippingWindow::isActive() const {
    return this->active;
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

EdgeState ClippingWindow::getState(WindowEdge boundary, Edge<float> edge) {
    auto classifyState = [](bool s_in, bool e_in) {
        return s_in ? (e_in ? IN_IN : IN_OUT)
                    : (e_in ? OUT_IN : OUT_OUT);
    };

    Point<float> start = edge.getStart();
    Point<float> end = edge.getEnd();

    switch (boundary) {
        case LEFT: {
            float minX = static_cast<float>(getMinX());
            return classifyState(start.x >= minX, end.x >= minX);
            break;
        }
        case RIGHT: {
            float maxX = static_cast<float>(getMaxX());
            return classifyState(start.x <= maxX, end.x <= maxX);
            break;
        }
        case BOT: {
            float minY = static_cast<float>(getMinY());
            return classifyState(start.y >= minY, end.y >= minY);
            break;
        }
        case TOP: {
            float maxY = static_cast<float>(getMaxY());
            return classifyState(start.y <= maxY, end.y <= maxY);
            break;
        }
    }

    return OUT_OUT;
}

Point<float> ClippingWindow::intersectEdge(WindowEdge boundary, Edge<float> edge) {
    Point<float> intersectPoint;
    Point<float> start = edge.getStart();
    Point<float> end = edge.getEnd();
    intersectPoint.rgb = start.rgb;

    float slope = 0.0f;
    bool vertical = (start.x == end.x);
    if (!vertical) {
        slope = (end.y - start.y) / (end.x - start.x);
    }

    switch (boundary) {
        case LEFT: {
            intersectPoint.x = static_cast<float>(getMinX());
            intersectPoint.y = vertical
                ? start.y
                : start.y + slope * (intersectPoint.x - start.x);
            break;
        }
        case RIGHT: {
            intersectPoint.x = static_cast<float>(getMaxX());
            intersectPoint.y = vertical
                ? start.y
                : start.y + slope * (intersectPoint.x - start.x);
            break;
        }
        case BOT: {
            intersectPoint.y = static_cast<float>(getMinY());
            intersectPoint.x = (slope == 0.0f)
                ? start.x
                : start.x + ((intersectPoint.y - start.y) / slope);
            break;
        }
        case TOP: {
            intersectPoint.y = static_cast<float>(getMaxY());
            intersectPoint.x = (slope == 0.0f)
                ? start.x
                : start.x + ((intersectPoint.y - start.y) / slope);
            break;
        }
    }

    return intersectPoint;
}

void ClippingWindow::clipSelection() {
    active = true;
    vector<Polygon> polys = Polygon::getPolys();

    for (const Polygon& curPoly : polys) {
        vector<Point<float>> newPoints;
        vector<Point<float>> oldPoints = toFloat(curPoly.getVertices());

        for (WindowEdge boundary = LEFT; boundary <= TOP; ++boundary) {
            if (oldPoints.empty()) break;

            for (vector<Point<float>>::iterator curPoint = oldPoints.begin(); curPoint != oldPoints.end() - 1; ++curPoint) {
                // TODO: increment color
                switch (getState(boundary, Edge<float>(*curPoint, *(curPoint + 1)))) {
                    case IN_IN: {
                        newPoints.push_back(*(curPoint + 1));
                        break;
                    }
                    case IN_OUT: {
                        newPoints.push_back(intersectEdge(boundary, Edge<float>(*curPoint, *(curPoint + 1))));
                        break;
                    }
                    case OUT_IN: {
                        newPoints.push_back(intersectEdge(boundary, Edge<float>(*curPoint, *(curPoint + 1))));
                        newPoints.push_back(*(curPoint + 1));
                        break;
                    }
                    default:
                        break;
                } 
            }

            Point<float> front = oldPoints.front();
            Point<float> back = oldPoints.back();
            switch (getState(boundary, Edge<float>(back, front))) {
                case IN_IN: {
                    newPoints.push_back(front);
                    break;
                }
                case IN_OUT: {
                    newPoints.push_back(intersectEdge(boundary, Edge<float>(back, front)));
                    break;
                }
                case OUT_IN: {
                    newPoints.push_back(intersectEdge(boundary, Edge<float>(back, front)));
                    newPoints.push_back(front);
                    break;
                }
                default:
                    break;
            } 
            
            std::reverse(newPoints.begin(), newPoints.end());
            oldPoints = std::move(newPoints);
        }

        vector<Point<int>> finalPoints = toInteger(oldPoints); 

        Polygon *newPolygon = Polygon::getCurrentOrCreate(true);
        for (const Point<int>& curPoint : finalPoints) {
            newPolygon->addVertex(curPoint);
        }
        newPolygon->finish();
    }
}