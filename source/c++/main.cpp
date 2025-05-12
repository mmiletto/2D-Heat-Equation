#include "mesh.h"

int main(int argc, char *argv[])
{
    mesh m;
    const int target_n_triangles = std::atoi(argv[1]);

    m.generate_square_mesh(target_n_triangles, 1.0);
    m.write_triangle_mesh();
}
