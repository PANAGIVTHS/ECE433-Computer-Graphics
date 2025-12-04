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

    // Instance members
    bool complete;
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
    static Polygon *addPolygon();

public:
    Polygon();
    void addVertex(Point<int> point);
    //TODO remve replace with addvrtx and finish
    void addLastVertex(Point<int> point); 
    void draw();
    int getMinY() const;
    int getMaxY() const;
    void polyFinish();
    vector<Point<int>> getVertices() const;
    
    // Static management functions
    static void clear();
    static Polygon& getPolygon(int i);
    static vector<Polygon>& getPolys();
    static int getTotalPolygons();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate();
};

#endif