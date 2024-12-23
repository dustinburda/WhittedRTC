//
// Created by Dustin on 11/26/24.
//

#include "../include/Mesh.h"

Mesh::Mesh(std::shared_ptr<MeshData> mesh_data)
    : curr_triangle_index_{-1}
{
    for(int index = 0; auto& face : mesh_data->faces_) {
        auto [f1, f2, f3] = face;
        triangles_.emplace_back(mesh_data->vertices_[f1], mesh_data->vertices_[f2], mesh_data->vertices_[f3]);
    }
}

Normal<double, 3> Mesh::NormalAt(const Point<double, 3>& p) const {
    if (curr_triangle_index_ == -1)
        return {-1.0, -1.0, -1.0};

    auto& triangle = triangles_[curr_triangle_index_];
    return triangle.NormalAt(p);

}

bool Mesh::Hit(const Ray& r, ShadeContext& context) const {
    bool hit = false;

    for(int index = 0; const auto& triangle : triangles_) {
        if(triangle.Hit(r, context)) {
            hit = true;
            curr_triangle_index_ = index;
        }
        index++;
    }

    return hit;
}