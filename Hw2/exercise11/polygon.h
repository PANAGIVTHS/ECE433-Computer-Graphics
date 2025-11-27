#ifndef POLYGON_H
#define POLYGON_H

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

class Polygon {
private:
    // Static members to manage the global list of polygons
    static Polygon **polys;
    static unsigned int totalPolys;
    static bool selectingPolygon;

    // Instance members
    Point *vertices;
    unsigned int totalVertices;

    // Helper to add a new polygon to the static list
    static Polygon *addPolygon();

public:
    Polygon();
    ~Polygon();

    void addVertex(Point point);
    void draw();

    // Static management functions
    static void init();
    static void destroy();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate();
    static bool completeCurrent(Point vertex);
};

#endif