
SUTHERLAND:

oldPoints = to_float(poly.getVertices()); // returns a vector of edges
newPoints = newVector.empty();

for (WindowEdge boundary = 0; boundary <= TOP; boundary++) {
    for (Point& curPoint : oldPoints) {
        curEdge = {curPoint, curPoint + 1};
        switch (ClippingWindow.getState(boundary, curEdge)) {
            case ININ: {
                newPoints.insert.to_front = curPoint + 1;
            }
            case INOUT: {
                newPoints.insert.to_front = interceptPoint;
            }
            case OUTIN: {
                newPoints.insert.to_front = interceptPoint;
                newPoints.insert.to_front = curPoint + 1;
            }
            case OUTOUT: {
                // Insert nothing
            }
        }
    }
    oldPoints = std::move(newPoints);
}

draw(to_int(oldPoints));


GET_STATE: Input: WindowEdge boundary, Edge edge

auto classifyState = [](bool s_in, bool e_in) {
    return s_in ? (e_in ? ININ : INOUT)
                : (e_in ? OUTIN : OUTOUT);
};

switch (boundary) {
    case LEFT: {
        return classifyState(edge.start.x >= clip.left.x,
                             edge.end.x >= clip.left.x);
    }
    case RIGHT: {
        return classifyState(edge.start.x <= clip.right.x,
                             edge.end.x <= clip.right.x);
    }
    case BOT: {
        return classifyState(edge.start.y >= clip.top.y,
                             edge.end.y >= clip.top.y);
    }
    case TOP: {
        return classifyState(edge.start.y <= clip.bot.y,
                             edge.end.y <= clip.bot.y);
    }
}



