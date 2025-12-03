EDGE:
struct {
    int maxY;
    float currentX, xIncr;
} Edge;

TABLE:
#numOfScanlines = ceil(floor(maxYpoly) - ceil(minYpoly)) - 1;
activeEdgeTable[#numOfScanlines];

for (int i = 0; i < #numOfScanlines; i++>) {
    (edge1, edge2, ...) = scanline(.y(i))
    activeEdgeTable[i] = newList(edge1, edge2, ...);
}

LIST:
activeEdgeList = newList();

for (int i = 0; i < #numOfScanlines; i++>) {
    for (each edge in activeEdgeList) {
        if (edge.maxY == i) {   
            removeElement(.list(activeEdgeList), .elements(edge))
        }
    }
    addElement(.list(activeEdgeList), .elements(activeEdgeTable[i]))
    fillCurrentLine(.y(i));
}

FILL_LINE: Input: y

edges[] = getElements(.list(activeEdgeList)); // #elements are mult 2
for (each successive pair of edges except last) {
    drawHorzLine(ceil(edges1.getX(y)), floor(edges2.getX(y)));
}
drawHorzLine(ceil(edges1.getX(y)), ceil(edges2.getX(y)) - 1);