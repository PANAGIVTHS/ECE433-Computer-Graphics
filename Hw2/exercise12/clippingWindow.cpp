#include "clippingWindow.h"

ClippingWindow::ClippingWindow(Point start, Point end) {
    this->start = start;
    this->end = end;
}

EdgeState ClippingWindow::getState(WindowEdge boundary, Edge edge) {
    auto classifyState = [](bool s_in, bool e_in) {
        return s_in ? (e_in ? IN_IN : IN_OUT)
                    : (e_in ? OUT_IN : OUT_OUT);
    };

    switch (boundary) {
        case LEFT: {
            return classifyState(edge.getStart().x >= this->left.x,
                                edge.getEnd().x >= this->left.x);
        }
        case RIGHT: {
            return classifyState(edge.getStart().x <= clip.right.x,
                                edge.getEnd().x <= clip.right.x);
        }
        case BOT: {
            return classifyState(edge.getStart().y <= clip.top.y,
                                edge.getEnd().x <= clip.top.y);
        }
        case TOP: {
            return classifyState(edge.getStart().y >= clip.bot.y,
                                edge.getEnd().x >= clip.bot.y);
        }
    }
}