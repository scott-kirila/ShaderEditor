//
// Created by hetan on 5/15/2024.
//

#ifndef VECTOR_H
#define VECTOR_H

struct Vector2Di {
    Vector2Di() : x(0), y(0) {}
    Vector2Di(const int _x, const int _y) : x(_x), y(_y) {}

    int x;
    int y;
};

struct Vector4Df {
    Vector4Df() : x(0), y(0), z(0), w(0) {}
    Vector4Df(const float _x, const float _y, const float _z, const float _w) :
    x(_x), y(_y), z(_z), w(_w) {}

    float x;
    float y;
    float z;
    float w;
};

#endif //VECTOR_H
