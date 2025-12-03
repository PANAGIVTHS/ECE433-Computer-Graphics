#include "polygon.h"
#include "edge.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
using namespace std;

// ----------------------------------- 
// Static fields
// -----------------------------------
vector<Polygon> Polygon::polys;
bool Polygon::selectingPolygon = false;

// ----------------------------------- 
// Private instance methods
// -----------------------------------

void Polygon::fillLine(int y) {
    int numEdges = (int) activeEdgeList.size();
    if (numEdges < 2) return; 

    for (int i = 0; i < numEdges; i += 2) {
        Edge& startEdge = activeEdgeList[i];
        Edge& endEdge = activeEdgeList[i + 1];
        RGB startColor = startEdge.getCurrentColor();
        RGB endColor = endEdge.getCurrentColor();
        int x1 = (int) ceil(startEdge.getCurrentX());
        int x2 = (int) floor(endEdge.getCurrentX());
        if (x1 > x2) continue;

        RGB currentColor = startColor;
        float rIncr = (float) (endColor.red - startColor.red) / (x2 - x1);
        float gIncr = (float) (endColor.green - startColor.green) / (x2 - x1);
        float bIncr = (float) (endColor.blue - startColor.blue) / (x2 - x1);

        glBegin(GL_POINTS);
        for (int j = x1; j <= x2; j++) {
            glColor3f(currentColor.red, currentColor.green, currentColor.blue);
            glVertex2i(j, y);

            currentColor.red += rIncr;
            currentColor.green += gIncr;
            currentColor.blue += bIncr;
        }
        glEnd();
    }
}

std::vector<Edge> Polygon::getEdges() {
    std::vector<Edge> edgesVec;

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        unsigned int j = (i + 1) % vertices.size();
        edgesVec.push_back(Edge(vertices[i], vertices[j]));
    }
    return edgesVec;
}

void Polygon::initActiveEdgeTable() {
    int numOfScanlines = getMaxY() - getMinY() + 1;
    activeEdgeTable.resize(numOfScanlines);
    
    int totalMinY = getMinY();
    std::vector<Edge> edgesVec = getEdges();

    for (Edge& curEdge : edgesVec) {
        int minY = curEdge.getMinY();
        int maxY = curEdge.getMaxY();
        curEdge.setHeight(numOfScanlines);

        //* Skip horizontal and if non crossing edges
        if (maxY - minY == 0)
            continue;

        activeEdgeTable.at(minY - totalMinY).push_back(curEdge);
    }
}

// ----------------------------------- 
// Private static methods
// -----------------------------------
Polygon* Polygon::addPolygon() {
    selectingPolygon = true;
    polys.push_back(Polygon());
    return &polys.back();
}

// ----------------------------------- 
// Public instance methods
// -----------------------------------
void Polygon::addVertex(Point point) {
    if (vertices.empty()) {
        totalMaxY = point.y;
        totalMinY = point.y;
    }

    totalMaxY = (point.y > totalMaxY) ? point.y : totalMaxY;
    totalMinY = (point.y < totalMinY) ? point.y : totalMinY;

    vertices.push_back(point);
}

int Polygon::getMinY() const {
    return totalMinY;
}

int Polygon::getMaxY() const {
    return totalMaxY;
}

void Polygon::drawLastVertex() {
    if (vertices.empty()) return;

    Point point = vertices.back();
    RGB color = point.rgb;
    glBegin(GL_POINTS);
    glColor3f(color.red, color.green, color.blue);
    glVertex2i(point.x, point.y);
    glEnd();
}

void Polygon::fill() {
    initActiveEdgeTable();

    int yStart = getMinY();
    int offset = 0;
    for (const std::vector<Edge>& curEdgeList : activeEdgeTable) {
        int scanlineY = yStart + offset;

        std::vector<Edge>::iterator curEdgeIt = activeEdgeList.begin();
        while (curEdgeIt != activeEdgeList.end()) {
            if (curEdgeIt->getMaxY() == scanlineY)
                curEdgeIt = activeEdgeList.erase(curEdgeIt);
            else
                ++curEdgeIt;
        }

        for (const Edge& newEdge : curEdgeList) {
            activeEdgeList.push_back(newEdge);
        }

        std::sort(activeEdgeList.begin(), activeEdgeList.end(), 
            [](const Edge& a, const Edge& b) {
                return a.getCurrentX() < b.getCurrentX();
            }
        );

        fillLine(scanlineY);

        for (Edge& newEdge : activeEdgeList) {
            newEdge.incrementX();
            newEdge.incrementColor();
        }
        offset++;
    }
}

// ----------------------------------- 
// Public static methods
// -----------------------------------
void Polygon::init() {
    selectingPolygon = false;
}

void Polygon::destroy() {
    polys.clear();
}

int Polygon::getTotalPolygons() {
    return polys.size();
}

Polygon *Polygon::getCurrent() {
    return &polys.back();
}

Polygon *Polygon::getCurrentOrCreate() {
    return selectingPolygon ? &polys.back() : addPolygon();
}

bool Polygon::completeCurrent(Point vertex) {
    if (selectingPolygon) {
        Polygon *cur = getCurrent();
        cur->addVertex(vertex);
        cur->drawLastVertex();
        cur->fill();
        glFlush();
    }
    bool oldVal = selectingPolygon;
    selectingPolygon = false;
    return oldVal;
}