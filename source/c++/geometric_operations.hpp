#pragma once
#include "mesh.h"

double dot(double2 a, double2 b)
{
    return a.x * b.x + a.y * b.y;
}

double dot(double2 a)
{
    return a.x * a.x + a.y * a.y;
}

double get_squared_distance(double2 a, double2 b)
{
    return dot(a - b);
}


double2 get_edge_mid_point(int v1, int v2, Triangle t, double2 *points)
{
    double2 mid_point {0, 0};
    auto a = t.get_vertex_coordinate(v1, points);
    auto b = t.get_vertex_coordinate(v2, points);

    mid_point.x = a.x < b.x ? a.x + (b.x - a.x) / 2.0 : b.x + (a.x - b.x) / 2.0;
    mid_point.y = a.y < b.y ? a.y + (b.y - a.y) / 2.0 : b.y + (a.y - b.y) / 2.0;

    return mid_point;
}
