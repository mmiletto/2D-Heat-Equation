#!/bin/bash
../cmake-build-debug/2D_heat_equation $1
python ../python/plot_mesh.py mesh_data.json