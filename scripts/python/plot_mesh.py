import numpy as np
import matplotlib.pyplot as plt
import matplotlib.tri as mtri
import json
import sys

filename = sys.argv[1]
data = None
with open(filename, "r") as file:
    print(f"Reading file: {filename}")
    data = json.load(file)
    print(data)

# Create a triangulation object
triangulation = mtri.Triangulation(data["x"], data["y"], data["triangles"])

# Plot the triangular mesh
plt.figure()
plt.triplot(triangulation, 'go-', lw=1.5, alpha=0.3)

# Annotate the vertices index
for i, p in enumerate(zip(data["x"], data["y"])):
    x = p[0]
    y = p[1]
    plt.text(x, y, str(i), fontsize=12, ha='right', color='blue')  # Add the index next to the point

plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Triangular Mesh')
plt.show()
