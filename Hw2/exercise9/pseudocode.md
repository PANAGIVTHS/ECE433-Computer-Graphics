TABLE:
minYpoly = minimum Y of all vertices of the polygon
maxYpoly = maximum Y of all vertices of the polygon

#numOfScanlines = maxYpoly - minYpoly + 1
activeEdgeTable[#numOfScanlines];

for (each edge e of the polygon) {
    minY = minimum Y of e;
    maxY = maximum Y of e;

    if (maxY == minY)
        continue;
    
    activeEdgeTable[minY - minYpoly] = e;
}

LIST:
activeEdgeList = newList();

for (int i = 0; i < #numOfScanlines; i++) {
    for (each edge in activeEdgeList) {
        if (edge.maxY == i + poly.minY) {   
            removeElements(activeEdgeList, edge);
        }
    }
    addElements(activeEdgeList, activeEdgeTable[i]);
    fillCurrentLine(i + poly.minY);
}

FILL_LINE: Input: y

edges[] = getElements(activeEdgeList);
for (each successive pair of edges except last) {
    drawHorzLine(ceil(edges1.getX(y)), floor(edges2.getX(y)));
}
drawHorzLine(ceil(edges1.getX(y)), ceil(edges2.getX(y)) - 1);