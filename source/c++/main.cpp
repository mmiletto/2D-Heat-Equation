#include "mesh.h"
#include <iostream>

int main(int argc, char *argv[])
{
    mesh m;
    std::cout << "Starting 2D Heat Equation solver" << std::endl;
    mesh triangle_mesh;
    const int target_n_triangles = std::atoi(argv[1]);
    triangle_mesh.generate_square_mesh(target_n_triangles, 1.0);
    triangle_mesh.compute_mesh_triangles_area();
    triangle_mesh.write_triangle_mesh();

    m.generate_square_mesh(target_n_triangles, 1.0);
    m.write_triangle_mesh();
    std::cout << "Terminating 2D Heat Equation solver" << std::endl;
}
