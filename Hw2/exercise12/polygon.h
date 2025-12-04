#ifndef POLYGON_H
#define POLYGON_H
#include "utilities.h"
#include "edge.h"
#include <vector>
#include <optional>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

class Polygon {
private:
    // Static members to manage the global list of polygons
    static vector<Polygon> polys;

    // Instance members
    bool complete, clipped;
    int totalMaxY, totalMinY;
    vector<Point<int>> vertices;
    vector<vector<Edge<int>>> activeEdgeTable;
    vector<Edge<int>> activeEdgeList;

    void fillLine(int y);
    vector<Edge<int>> getEdges();
    void initActiveEdgeTable();
    void drawVertices();
    void fill();

    // Helper to add a new polygon to the static list
    static Polygon *addPolygon(bool clipped);

public:
    Polygon(bool clipped);
    void addVertex(Point<int> point);
    void finish(Point<int> lastPoint); 
    void finish();
    void draw(bool fill);
    int getMinY() const;
    int getMaxY() const;
    vector<Point<int>> getVertices() const;
    bool isClipped() const;
    
    // Static management functions
    static void clear();
    static void clearClipped();
    static vector<Polygon>& getPolys();
    static Polygon& getPolygon(int i);
    static int getTotalPolygons();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate(bool clipped);
};

#endif