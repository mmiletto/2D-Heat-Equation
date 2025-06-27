#include "mesh.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Starting 2D Heat Equation solver" << std::endl;

    // Steps
    // 1. Preprocessing
    // 1.1 Generate the triangular mesh
    mesh triangle_mesh;
    const int target_n_triangles = std::atoi(argv[1]);
    // Generate a 1mx1m square triangulated mesh, containing target_n_triangles
    triangle_mesh.generate_square_mesh(target_n_triangles, 1.0);
    triangle_mesh.compute_mesh_triangles_area();
    triangle_mesh.write_triangle_mesh();
    // 1.2 Represent the connectivity matrix as COO format

    // 1.3 Compute triangles area

    // 1.4 Load initial conditions
    // Initial temperature, boundary condition, material properties, solver timestep

    // Solver start

    // 2. Assembly the local and global stiffness and mass matrices for FEM solver

    // 3. Apply boundary conditions

    // 4. Solve the system

    // 5. Post-processing

    std::cout << "Terminating 2D Heat Equation solver" << std::endl;
}
