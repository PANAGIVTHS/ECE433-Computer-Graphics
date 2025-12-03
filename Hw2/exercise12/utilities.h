#ifndef UTILITIES_H
#define UTILITIES_H

typedef struct {
    float red, green, blue;
} RGB;

typedef struct {
    int x, y;
    RGB rgb;
} Point;

//TODO with generic typename
typedef struct {
    float x, y;
    RGB rgb;
} PointFloat;

typedef enum {
    LEFT,
    RIGHT,
    BOT,
    TOP
} WindowEdge;

typedef enum {
    OUT_OUT,
    OUT_IN,
    IN_OUT,
    IN_IN
} EdgeState;

#endif