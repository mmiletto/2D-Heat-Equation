#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>

#include "geometric_operations.hpp"
#include "mesh.h"
/*!
 * Given a square of size 1x1m and two triangles, decompose it into a mesh of n triangles by subdividing each triangle into two.
 * This method removes duplicated vertices based on proximity.
 * @param [in] target_n_triangles The target number of triangles for the final mesh to have.
 * @param [in] size The size, in meters for the mesh to have.
 */
void mesh::generate_square_mesh(const int target_n_triangles, const double size)
{
    // Contains the double coordinates mapped to a vertex/edge index, considering 8 decimal places (like a hash function)
    std::map<int, std::map<int, int>> existing_coordinates_map;
    const int precision = 8;

    // Check if the newly create vertex already exists and return its index. If no such vertex exists, return -1.
    auto find_existing_vertex = [&existing_coordinates_map](double2 vertex) -> int
    {
        const int x = coord_to_int(vertex.x);
        if (existing_coordinates_map.contains(x))
        {
            const int y = coord_to_int(vertex.y);
            if (existing_coordinates_map[x].contains(y))
            {
                // return the mapped vertex index of the given coordinate
                return existing_coordinates_map[x][y];
            }
        }
        return -1;
    };

    auto add_vertex_to_map = [&existing_coordinates_map](double2 vertex, const int vertex_id) -> void
    {
        const int x = coord_to_int(vertex.x);
        const int y = coord_to_int(vertex.y);
        if (!existing_coordinates_map.contains(x))
        {
            existing_coordinates_map[x] = {{y, vertex_id}};
        }
        else if (!existing_coordinates_map[x].contains(y))
        {
            existing_coordinates_map[x][y] = vertex_id;
        }
    };

    std::vector<double2> initial_points = {{0.0, 0.0}, {0.0, size}, {size, 0.0}, {size, size}};
    std::vector<Triangle> initial_triangles = {{0, 1, 2}, {1, 2, 3}};
    // Register the initial points to the vertexes mapping
    for (int i=0; i<initial_points.size(); i++)
        add_vertex_to_map(initial_points[i], i);

    // For each triangle find the longest edge and create a new point in its middle
    // Join this new point with the vertex that is not a part of the longest edge + one of the vertices that
    // is part of the longest edge to create a new triangle
    int triangle_start = 0;
    size_t initial_triangles_size = initial_triangles.size();
    size_t current_n_triangles = initial_triangles.size();
    while (current_n_triangles < target_n_triangles)
    {
        const Triangle t = initial_triangles[triangle_start];
        auto [v1, v2] = t.get_longest_edge_pair(initial_points.data());
        auto mid_point = get_edge_mid_point(v1, v2, t, initial_points.data());
        auto v3 = t.get_remaining_vertex(v1, v2);

        // From the new point created and the unused vertex, override the triangle_start and create a new one
        int mid_point_index = (int) initial_points.size();
        // Check if the point exists
        const int vertex_index = find_existing_vertex(mid_point);
        if (vertex_index > 0)
        {
            mid_point_index = vertex_index;
        }
        else
        {
            add_vertex_to_map(mid_point, mid_point_index);
            initial_points.push_back(mid_point);
        }

        // Create a new triangle using v2
        initial_triangles.push_back(Triangle{t.vertices[v2], t.vertices[v3], mid_point_index});

        // Update existing triangle vertices dropping v2 and using v1 and mid-point
        initial_triangles[triangle_start].vertices[0] = t.vertices[v1];
        initial_triangles[triangle_start].vertices[1] = t.vertices[v3];
        initial_triangles[triangle_start].vertices[2] = mid_point_index;

        triangle_start++;
        current_n_triangles = initial_triangles.size();

        // When we finish a round of triangle divisions, go back to the first one
        if (triangle_start >= initial_triangles_size)
        {
            triangle_start = 0;
            initial_triangles_size = current_n_triangles;
        }
    }

    this->vertexes = initial_points;
    this->triangles = initial_triangles;
}

void mesh::write_triangle_mesh() const
{
    // Print coordinates:
    nlohmann::json data;

    for (auto point : this->vertexes)
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


void mesh::setup_initial_temperature(double temperature)
{
}

/*!
 * Convert a floating point coordinate value into its integer representation.
 * @param [in] coord The coordinate to be transformed
 * @param precision Desired number of decimal places to consider for the rounding
 * @return The coordinate int representation given a the decimal precision.
 */
int mesh::coord_to_int(double coord, int precision)
{
    const double multiplier = std::pow(10.0, precision);
    return static_cast<int>(std::round(coord * multiplier));
}

double2 Triangle::get_vertex_coordinate(int vertex, double2 *points) const
{
    return points[this->vertices[vertex]];
}

std::pair<int, int> Triangle::get_longest_edge_pair(double2 *points) const
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
