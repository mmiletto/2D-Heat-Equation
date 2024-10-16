#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <sstream>

#include "geometric_operations.hpp"
#include "mesh.h"
/*!
 * Given a square of size 1x1m and two triangles, decompose it into a mesh of n triangles by subdividing each triangle into two.
 * @param n_triangles
 */
void mesh::generate_square_mesh(const int n_triangles)
{
    std::vector<double2> initial_points = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    std::vector<Triangle> initial_triangles = {{0, 1, 2}, {1, 2, 3}};

    // For each triangle find the longest edge and create a new point in its middle
    // Join this new point with the vertex that is not a part of the longest edge + one of the vertices that
    // is part of the longest edge to create a new triangle
    int triangle_start = 0;
    size_t initial_triangles_size = initial_triangles.size();
    size_t n_tria = initial_triangles.size();
    while (n_tria < n_triangles)
    {
        auto &t = initial_triangles[triangle_start];
        std::cout << "n_tria " << n_tria << " triangle_start " << triangle_start <<
                  " initial_triangles_size " << initial_triangles_size << std::endl;

        auto [v1, v2] = t.get_longest_edge_pair(initial_points.data());
        auto mid_point = get_edge_mid_point(v1, v2, t, initial_points.data());
        auto v3 = t.get_remaining_vertex(v1, v2);
        std::cout << " mid point for v1 v2 = " << mid_point.x << " " << mid_point.y << std::endl;

        // From the new point created and the unused vertex, override the triangle_start and create a new one
        int mid_point_index = (int) initial_points.size();
        initial_points.push_back(mid_point);

        // Create new triangle using v2
        initial_triangles.push_back(Triangle{t.vertices[v2], t.vertices[v3], mid_point_index});

        // Update existing triangle
        t.vertices[0] = t.vertices[v1];
        t.vertices[1] = t.vertices[v3];
        t.vertices[2] = mid_point_index;

        triangle_start++;
        n_tria = initial_triangles.size();

        // When we finish a round of triangle divisions, go back to the first one
        if (triangle_start >= initial_triangles_size)
        {
            triangle_start = 0;
            initial_triangles_size = n_tria;
        }
    }

    this->points = initial_points;
    this->triangles = initial_triangles;
}

void mesh::write_triangle_mesh() const
{
    // Print coordinates:
    nlohmann::json data;

    for (auto point : this->points)
    {
        data["x"].push_back(point.x);
        data["y"].push_back(point.y);
    }

    for (auto triangle : this->triangles)
    {
        data["triangles"].push_back({triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]});
    }

    std::ofstream out_triangles("mesh_data.json");
    if (out_triangles.is_open())
    {
        out_triangles << data.dump(4);
        out_triangles.close();
    }
    else
    {
        std::cerr << "Failed to open file\n";
    }
}

double2 Triangle::get_vertex_coordinate(int vertex, double2 *points)
{
    return points[this->vertices[vertex]];
}

std::pair<int, int> Triangle::get_longest_edge_pair(double2 *points)
{
    double max_distance{0.0};
    int _v1, _v2;
    auto get_edge_length_and_compare = [&](int vertex_1, int vertex_2)
    {
        auto distance = get_squared_distance(this->get_vertex_coordinate(vertex_1, points),
                                             this->get_vertex_coordinate(vertex_2, points));
        if (distance > max_distance)
        {
            _v1 = vertex_1;
            _v2 = vertex_2;
            max_distance = distance;
        }
    };

    get_edge_length_and_compare(0, 1);
    get_edge_length_and_compare(0, 2);
    get_edge_length_and_compare(1, 2);

    return {_v1, _v2};
}

int Triangle::get_remaining_vertex(int vertex_1, int vertex_2) const
{
    // Possible vertex combinations sum
    // 0 1 - 1
    // 0 2 - 2
    // 1 2 - 3
    return 3 - (vertex_1 + vertex_2);
}
