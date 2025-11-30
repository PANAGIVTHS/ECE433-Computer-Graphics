#ifndef POLYGON_H
#define POLYGON_H

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

typedef struct {
    Point start, end;

    //TODO Katw ta xeria sou Jason START
    inline int getMinX() const {
        return (start.x < end.x ? start.x : end.x);
    }
    inline int getMaxX() const {
        return (start.x > end.x ? start.x : end.x);
    }
    inline int getMinY() const {
        return (start.y < end.y ? start.y : end.y);
    }
    inline int getMaxY() const {
        return (start.y > end.y ? start.y : end.y);
    }
    //TODO Katw ta xeria sou Jason END
} Edge;

class Polygon {
private:
    // Static members to manage the global list of polygons
    static Polygon **polys;
    static unsigned int totalPolys;
    static bool selectingPolygon;

    // Instance members
    Point *vertices;
    unsigned int totalVertices;
    int totalMaxY;
    int totalMinY;
    bool isFirstVertex;

    // Helper to add a new polygon to the static list
    static Polygon *addPolygon();

public:
    Polygon();
    ~Polygon();

    void addVertex(Point point);
    void draw();
    int getMinY() const;
    int getMaxY() const;
    std::vector<Edge> getEdges() const;
    std::vector<Edge> getEdgesCrossing(int y) const;
    
    // Static management functions
    static void init();
    static void destroy();
    static Polygon *getCurrent();
    static Polygon *getCurrentOrCreate();
    static bool completeCurrent(Point vertex);
};

#endif