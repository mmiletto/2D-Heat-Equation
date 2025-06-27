#pragma once
#include <vector>

struct double2
{
    // two coordinates that define a point
    double x, y;

    double2 operator-(double2 other) const
    {
        return {this->x - other.x, this->y - other.y};
    }
};

struct Triangle
{
    // three vertices index that form a triangle
    int vertices[3];
    double area;

    double2 get_vertex_coordinate(int vertex, double2 *points) const;
    std::pair<int, int> get_longest_edge_pair(double2 *points) const;
    int get_remaining_vertex(int vertex_1, int vertex_2) const;
    double compute_area(const double2* points);
};


/*!
 * Represents the triangular mesh connectivity using the COO format.
 */
class ConnectivityMatrix
{
public:
    void setup(const std::vector<Triangle>& triangles);

private:
    std::vector<int> rows;
    std::vector<int> columns;
    std::vector<double> temperature;
};

class mesh
{
public:
    void generate_square_mesh(const int target_n_triangles, const double size);
    void compute_mesh_triangles_area();
    static int coord_to_int(double coord, int precision = 8);

    // Initial conditions
    void setup_initial_temperature(double temperature);
    void generate_connectivity_matrix();

    // Post-processing functions
    void write_triangle_mesh() const;

private:
    // List of vertex positions. They define the triangles' geometry.
    std::vector<double2> vertices;
    // List of triangle structure, each triangle is composed by three vertices indexes.
    std::vector<Triangle> triangles;

    // mesh connectivity matrix, mapping the vertices connectivity
    ConnectivityMatrix matrix;
};
