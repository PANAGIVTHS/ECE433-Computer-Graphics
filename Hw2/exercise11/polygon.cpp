#include "polygon.h"
#include <stdlib.h>
#include <vector> 
#include <GL/glut.h>

// ----------------------------------- 
// Static fields
// -----------------------------------
Polygon **Polygon::polys = NULL;
unsigned int Polygon::totalPolys = 0;
bool Polygon::selectingPolygon = false;

// ----------------------------------- 
// Constructor and destructor
// -----------------------------------
Polygon::Polygon() {
    vertices = NULL;
    totalVertices = 0;
    isFirstVertex = false;
}

Polygon::~Polygon() {
    free(vertices);
}

// ----------------------------------- 
// Public instance methods
// -----------------------------------
void Polygon::addVertex(Point point) {
    if (isFirstVertex) {
        totalMaxY = point.y;
        totalMinY = point.x;
        isFirstVertex = false;
    }

    totalMaxY = (point.y > totalMaxY) ? point.y : totalMaxY;
    totalMinY = (point.y < totalMinY) ? point.y : totalMinY;

    vertices = (Point *) realloc(vertices, (totalVertices + 1) * sizeof(Point));
    vertices[totalVertices++] = point;
}

int getMinY() const {
    return(totalMinY);
}

int getMaxY() const {
    return(totalMaxY);
}

std::vector<Edge> Polygon::getEdges() const {
    std::vector<Edge> edgesVec;
    edgesVec.reserve(totalVertices);

    for (unsigned int i = 0; i < totalVertices; ++i) {
        unsigned int j = (i + 1) % totalVertices;
        edgesVec.push_back({vertices[i], vertices[j]});
    }
    return edgesVec;
}

std::vector<Edge> Polygon::getEdgesCrossing(int y) const {
    std::vector<Edge> edgeVecCross;
    
    if (y < totalMinY || y > totalMaxY) {
        return edgeVecCross;
    }

    std::vector<Edge> edgesVec = getEdges();
    edgeVecCross.reserve(edgesVec.size());

    for (const Edge& curEdge : edgesVec) {
        int minY = curEdge.getMinY();
        int maxY = curEdge.getMaxY();

        //* Skip horizontal and if non crossing edges
        if (maxY < y || minY > y || (maxY - minY == 0)) 
            continue;

        edgeVecCross.push_back(curEdge);

    }

    return edgeVecCross;
}

void Polygon::draw() {
    if (totalVertices <= 1) return;

    Point first = selectingPolygon ? vertices[totalVertices - 2] : vertices[totalVertices - 1];
    RGB firstColor = first.rgb;
    Point second = selectingPolygon ? vertices[totalVertices - 1] : vertices[0];
    RGB secondColor = second.rgb;
    glBegin(GL_LINES);
    glColor3f(firstColor.red, firstColor.green, firstColor.blue);
    glVertex2i(first.x, first.y);
    glColor3f(secondColor.red, secondColor.green, secondColor.blue);
    glVertex2i(second.x, second.y);
    glEnd();
}

// ----------------------------------- 
// Public static methods
// -----------------------------------
void Polygon::init() {
    polys = (Polygon **) malloc(sizeof(Polygon *));
    totalPolys = 0;
    selectingPolygon = false;
}

void Polygon::destroy() {
    for (int i = 0; i < totalPolys; i++) {
        Polygon *poly = polys[i];
        poly->~Polygon();
        free(poly);
    }
    free(polys);
}

Polygon *Polygon::getCurrent() {
    return totalPolys != 0 ? polys[totalPolys - 1] : NULL;
}

Polygon *Polygon::getCurrentOrCreate() {
    return selectingPolygon ? polys[totalPolys - 1] : addPolygon();
}

bool Polygon::completeCurrent(Point vertex) {
    if (selectingPolygon) {
        Polygon *cur = polys[totalPolys - 1];
        cur->addVertex(vertex);
        cur->draw();
    }
    bool oldVal = selectingPolygon;
    selectingPolygon = false;
    return oldVal;
}

// ----------------------------------- 
// Private static methods
// -----------------------------------
Polygon *Polygon::addPolygon() {
    selectingPolygon = true;
    polys = (Polygon **) realloc(polys, (++totalPolys) * sizeof(Polygon *));
    Polygon *newPolygon = (Polygon *) malloc(sizeof(Polygon));
    *newPolygon = Polygon();
    return polys[totalPolys - 1] = newPolygon;
}