#ifndef UTILITIES_H
#define UTILITIES_H
#include <vector>
using namespace std;

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

//TODO with generic typename
typedef struct {
    float x, y;
    RGB rgb;
} PointFloat;

typedef enum {
    LEFT,
    RIGHT,
    BOT,
    TOP
} WindowEdge;

typedef enum {
    OUT_OUT,
    OUT_IN,
    IN_OUT,
    IN_IN
} EdgeState;

vector<PointFloat> toFloat(const vector<Point>& points) {
    vector<PointFloat> pointsF;
    pointsF.reserve(points.size());

    for (const Point& curPoint : points) {
        pointsF.push_back({static_cast<float>(curPoint.x), static_cast<float>(curPoint.y), curPoint.rgb});
    }

    return pointsF;
}

vector<Point> toInteger(vector<PointFloat> pointsF) {
    vector<Point> points;
    points.reserve(pointsF.size());

    for (const PointFloat& curPoint : pointsF) {
        points.push_back({static_cast<int>(curPoint.x), static_cast<int>(curPoint.y), curPoint.rgb});
    }

    return points;
}

#endif