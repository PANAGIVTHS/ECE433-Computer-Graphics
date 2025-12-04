#include "clippingWindow.h"

template <typename T>
ClippingWindow<T>::ClippingWindow(Point<int> start, Point<int> end) {
    this->start = start;
    this->end = end;
}

template <typename T>
void ClippingWindow<T>::setEnd(Point<int> end) {
    this->end = end;
}

template <typename T>
void ClippingWindow<T>::draw() const {
    glRecti(start.x, start.y, end.x, end.y);
}

template <typename T>
bool ClippingWindow<T>::isActive() const {
    return this->active;
}

template <typename T>
int ClippingWindow<T>::getMinY() const {
    return start.y < end.y ? start.y : end.y;
}

template <typename T>
int ClippingWindow<T>::getMaxY() const {
    return start.y > end.y ? start.y : end.y;
}

template <typename T>
int ClippingWindow<T>::getMinX() const {
    return start.x < end.x ? start.x : end.x;
}

template <typename T>
int ClippingWindow<T>::getMaxX() const {
    return start.x > end.x ? start.x : end.x;
}

template <typename T>
EdgeState ClippingWindow<T>::getState(WindowEdge boundary, Edge<float> edge) {
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

template <typename T>
Point<float> ClippingWindow<T>::intersectEdge(WindowEdge boundary, Edge<float> edge) {
    Point<float> intersectPoint;
    Point<float> start = edge.getStart();
    Point<float> end = edge.getEnd();
    //TODO color

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


//TODONOW FIX Something with vector returning empty algorithmic probably  
template <typename T>
void ClippingWindow<T>::clipSelection() {
    active = true;
    vector<Polygon>& polys = Polygon::getPolys();
    for (const Polygon& curPoly : polys) {
        //TODO remove after testing
        
        vector<Point<float>> newPoints;
        vector<Point<float>> oldPoints = toFloat(curPoly.getVertices());
        for (Point<float>& curPoint : oldPoints) {
            curPoint.rgb = {1.0f, 0.0, 0.0};
        }

        for (WindowEdge boundary = LEFT; boundary <= TOP; ++boundary) {
            
            newPoints.clear(); // TODO this i think move does?
            if (oldPoints.empty()) break;

            for (vector<Point<float>>::iterator curPoint = oldPoints.begin(); curPoint != oldPoints.end() - 1; ++curPoint) {
                // TODO: increment color
                switch (getState(boundary, Edge<float>(*curPoint, *(curPoint + 1)))) {
                    case IN_IN: {
                        newPoints.insert(newPoints.begin(), *(curPoint + 1));
                        break;
                    }
                    case IN_OUT: {
                        newPoints.insert(newPoints.begin(), intersectEdge(boundary, Edge<float>(*curPoint, *(curPoint + 1))));
                        break;
                    }
                    case OUT_IN: {
                        newPoints.insert(newPoints.begin(), intersectEdge(boundary, Edge<float>(*curPoint, *(curPoint + 1))));
                        newPoints.insert(newPoints.begin(), *(curPoint + 1));
                        break;
                    }
                    case OUT_OUT: //! Noop
                    default:
                        break;
                } 
            }
            oldPoints = std::move(newPoints);
        }

        //TODO draw call this or make seperate array of these? idk bruh
        vector<Point<int>> finalPoints = toInteger(oldPoints); 

        for (const Point<int>& curPoint : finalPoints) {
            Polygon::getCurrentOrCreate(true)->addVertex(curPoint);
        }
        Polygon::getCurrent()->finish();
    }
}

template class ClippingWindow<int>;