#include "polygon.h"
#include "edge.h"
#include <stdlib.h>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <algorithm>
using namespace std;

// ----------------------------------- 
// Static fields
// -----------------------------------
vector<Polygon> Polygon::polys;

// ----------------------------------- 
// Private instance methods
// -----------------------------------

void Polygon::fillLine(int y) {
    int numEdges = (int) activeEdgeList.size();
    if (numEdges < 2) return; 

    for (int i = 0; i < numEdges; i += 2) {
        Edge<int>& startEdge = activeEdgeList[i];
        Edge<int>& endEdge = activeEdgeList[i + 1];
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

std::vector<Edge<int>> Polygon::getEdges() {
    std::vector<Edge<int>> edgesVec;

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        unsigned int j = (i + 1) % vertices.size();
        edgesVec.push_back(Edge<int>(vertices[i], vertices[j]));
    }
    return edgesVec;
}

void Polygon::initActiveEdgeTable() {
    if (vertices.empty()) return;
    int numOfScanlines = getMaxY() - getMinY() + 1;
    activeEdgeTable.clear();
    activeEdgeTable.resize(numOfScanlines);
    
    int totalMinY = getMinY();
    std::vector<Edge<int>> edgesVec = getEdges();

    for (Edge<int>& curEdge : edgesVec) {
        int minY = curEdge.getMinY();
        int maxY = curEdge.getMaxY();

        //* Skip horizontal and if non crossing edges
        if (maxY - minY == 0)
            continue;

        activeEdgeTable.at(minY - totalMinY).push_back(curEdge);
    }
}

void Polygon::drawVertices() {
    glBegin(GL_POINTS);
    for (Point<int>& point : vertices) {
        RGB color = point.rgb;
        glColor3f(color.red, color.green, color.blue);
        glVertex2i(point.x, point.y);
    }
    glEnd();
}

void Polygon::fill() {
    if (!this->complete) return;
    initActiveEdgeTable();

    int yStart = getMinY();
    int offset = 0;
    for (const std::vector<Edge<int>>& curEdgeList : activeEdgeTable) {
        int scanlineY = yStart + offset;

        std::vector<Edge<int>>::iterator curEdgeIt = activeEdgeList.begin();
        while (curEdgeIt != activeEdgeList.end()) {
            if (curEdgeIt->getMaxY() == scanlineY)
                curEdgeIt = activeEdgeList.erase(curEdgeIt);
            else
                ++curEdgeIt;
        }

        for (const Edge<int>& newEdge : curEdgeList) {
            activeEdgeList.push_back(newEdge);
        }

        std::sort(activeEdgeList.begin(), activeEdgeList.end(), 
            [](const Edge<int>& a, const Edge<int>& b) {
                return a.getCurrentX() < b.getCurrentX();
            }
        );

        fillLine(scanlineY);

        for (Edge<int>& newEdge : activeEdgeList) {
            newEdge.incrementX();
            newEdge.incrementColor();
        }
        offset++;
    }
}

// ----------------------------------- 
// Private static methods
// -----------------------------------
Polygon* Polygon::addPolygon(bool clipped) {
    polys.push_back(Polygon(clipped));
    return &polys.back();
}

// ----------------------------------- 
// Public instance methods
// -----------------------------------
Polygon::Polygon(bool clipped) {
    this->clipped = clipped;
    this->complete = false;
}

vector<Point<int>> Polygon::getVertices() const {
    return vertices;
}

void Polygon::addVertex(Point<int> point) {
    if (complete) return;
    if (vertices.empty()) {
        totalMaxY = point.y;
        totalMinY = point.y;
    }

    totalMaxY = (point.y > totalMaxY) ? point.y : totalMaxY;
    totalMinY = (point.y < totalMinY) ? point.y : totalMinY;

    vertices.push_back(point);
}

void Polygon::finish(Point<int> point) {
    addVertex(point);
    complete = true;
}

void Polygon::finish() {
    this->complete = true;
}

int Polygon::getMinY() const {
    return totalMinY;
}

int Polygon::getMaxY() const {
    return totalMaxY;
}

bool Polygon::isClipped() const {
    return clipped;
}

void Polygon::draw() {
    drawVertices();
    if (complete)
        fill();
}

// ----------------------------------- 
// Public static methods
// -----------------------------------
void Polygon::clear() {
    polys.clear();
}

Polygon& Polygon::getPolygon(int i) {
    return polys.at(i);
}

int Polygon::getTotalPolygons() {
    return polys.size();
}

Polygon *Polygon::getCurrent() {
    return polys.size() > 0 && !polys.back().complete ? &polys.back() : NULL;
}

Polygon *Polygon::getCurrentOrCreate(bool clipped) {
    return polys.size() > 0 && !polys.back().complete ? &polys.back() : addPolygon(clipped);
}

vector<Polygon>& Polygon::getPolys() {
    return polys;
}
