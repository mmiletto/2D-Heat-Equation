#include "mesh.h"

int main(int argc, char *argv[])
{
    mesh m;
    m.generate_square_mesh(std::atoi(argv[1]));
    m.write_triangle_mesh();
}
