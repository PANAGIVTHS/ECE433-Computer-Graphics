#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <optional>
#include <cmath>
#include "utilities.h"
#include "edge.h"
using namespace std;

class Polygon {
private:
    // Static members to manage the global list of polygons
    static vector<Polygon> polys;
    static bool selectingPolygon;

    // Instance members
    int totalMaxY, totalMinY;
    vector<Point> vertices;
    vector<vector<Edge>> activeEdgeTable;
    vector<Edge> activeEdgeList;

    void fillLine(int y);
    vector<Edge> getEdges();
    void initActiveEdgeTable();
    
    // Helper to add a new polygon to the static list
    static Polygon *addPolygon();

public:
    void addVertex(Point point); //TODO shouldnt add if already exists
    void fill(); 
    void drawLastVertex();
    int getMinY() const;
    int getMaxY() const;
    
    // Static management functions
    static void init();
    static void destroy();
    static int getTotalPolygons();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate();
    static bool completeCurrent(Point vertex);
};

#endif