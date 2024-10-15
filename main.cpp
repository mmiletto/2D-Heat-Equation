#include "mesh.h"

int main()
{
    mesh m;
    m.generate_square_mesh(30);
    m.write_triangle_mesh();
}
