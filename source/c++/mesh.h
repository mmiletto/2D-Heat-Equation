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

    double2 get_vertex_coordinate(int vertex, double2 *points) const;
    std::pair<int, int> get_longest_edge_pair(double2 *points) const;
    int get_remaining_vertex(int vertex_1, int vertex_2) const;
};

class mesh
{
public:
    void generate_square_mesh(const int target_n_triangles, const double size);
    void write_triangle_mesh() const;

    // Initial conditions
    void setup_initial_temperature(double temperature);
    void generate_connectivity_matrix();

    static int coord_to_int(double coord, int precision = 8);

private:
    std::vector<double2> vertexes;
    std::vector<Triangle> triangles;
};
