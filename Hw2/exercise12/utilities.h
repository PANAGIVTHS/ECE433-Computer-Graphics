#ifndef UTILITIES_H
#define UTILITIES_H
#include <vector>
using namespace std;

typedef struct {
    float red, green, blue;
} RGB;

//TODO with generic typename
template <typename T>
struct Point {
    T x;
    T y;
    RGB rgb;
};

typedef enum {
    LEFT = 0,
    RIGHT = 1,
    BOT = 2,
    TOP = 3
} WindowEdge;

inline WindowEdge& operator++(WindowEdge& e) {
    e = static_cast<WindowEdge>((static_cast<int>(e) + 1) % 4);
    return e;
}

typedef enum {
    OUT_OUT,
    OUT_IN,
    IN_OUT,
    IN_IN
} EdgeState;

inline vector<Point<float>> toFloat(const vector<Point<int>>& points) {
    vector<Point<float>> pointsF;
    pointsF.reserve(points.size());

    for (const Point<int>& curPoint : points) {
        pointsF.push_back({static_cast<float>(curPoint.x), static_cast<float>(curPoint.y), curPoint.rgb});
    }

    return pointsF;
}

inline vector<Point<int>> toInteger(const vector<Point<float>>& pointsF) {
    vector<Point<int>> points;
    points.reserve(pointsF.size());

    for (const Point<float>& curPoint : pointsF) {
        points.push_back({static_cast<int>(curPoint.x), static_cast<int>(curPoint.y), curPoint.rgb});
    }

    return points;
}

#endif