#ifndef POLYGON_H
#define POLYGON_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <optional>
#include <cmath>
#include <algorithm>
#include "utilities.h"
#include "edge.h"
using namespace std;

class Polygon {
private:
    // Static members to manage the global list of polygons
    static vector<Polygon> polys;

    // Instance members
    bool complete;
    int totalMaxY, totalMinY;
    vector<Point> vertices;
    vector<vector<Edge>> activeEdgeTable;
    vector<Edge> activeEdgeList;

    void fillLine(int y);
    vector<Edge> getEdges();
    void initActiveEdgeTable();
    void drawVertices();
    void fill();

    // Helper to add a new polygon to the static list
    static Polygon *addPolygon();

public:
    Polygon();
    void addVertex(Point point);
    void addLastVertex(Point point);
    void draw();
    int getMinY() const;
    int getMaxY() const;
    
    // Static management functions
    static void clear();
    static Polygon& getPolygon(int i);
    static int getTotalPolygons();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate();
};

#endif