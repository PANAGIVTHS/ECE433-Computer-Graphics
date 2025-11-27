#include "polygon.h"
#include <stdlib.h>
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
}

Polygon::~Polygon() {
    free(vertices);
}

// ----------------------------------- 
// Public instance methods
// -----------------------------------
void Polygon::addVertex(Point point) {
    vertices = (Point *) realloc(vertices, (totalVertices + 1) * sizeof(Point)); //FIX THIS
    vertices[totalVertices++] = point;
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

Polygon * Polygon::getCurrent() {
    return totalPolys != 0 ? polys[totalPolys - 1] : NULL;
}

Polygon * Polygon::getCurrentOrCreate() {
    return selectingPolygon ? polys[totalPolys - 1] : addPolygon();
}

bool Polygon::completeCurrent(Point vertex) {
    bool oldVal = selectingPolygon;
    selectingPolygon = false;
    return oldVal;
}

// ----------------------------------- 
// Private static methods
// -----------------------------------
Polygon * Polygon::addPolygon() {
    selectingPolygon = true;
    polys = (Polygon **) realloc(polys, (++totalPolys) * sizeof(Polygon *));
    Polygon *newPolygon = (Polygon *) malloc(sizeof(Polygon));
    *newPolygon = Polygon();
    return polys[totalPolys - 1] = newPolygon;
}