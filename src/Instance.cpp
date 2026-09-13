//
// Created by Dustin on 12/1/24.
//

#include "../include/Instance.h"

#include "../include/Mesh.h"

Instance::Instance(std::shared_ptr<ShapeInterface> shape, std::shared_ptr<MaterialInterface> mat, InstanceType type)
    : transform_{nullptr}, shape_{std::move(shape)}, mat_{std::move(mat)}, instance_type_{type} {}

Instance::Instance(std::shared_ptr<Transformation> t, std::shared_ptr<ShapeInterface> shape, std::shared_ptr<MaterialInterface> mat, InstanceType type)
    :transform_{t}, shape_{std::move(shape)}, mat_{std::move(mat)}, instance_type_{type} {}

Normal<double, 3> Instance::NormalAt(const Point<double, 3>& p) const {
    if(transform_ == nullptr)
        return shape_->NormalAt(p);

    return (*transform_)(shape_->NormalAt(p));
};

bool Instance::Hit(const Ray& r, ShadeContext& context) const {
    if (!BBox().Hit(r, context))
        return false;

    if (transform_ == nullptr)
    {
        if(shape_->Hit(r, context)){
            context.mat_ = mat_;
            return true;
        }

       return false;
    }

    Ray transformed_ray = transform_->ApplyInverse(r);

    if(!shape_->Hit(transformed_ray, context))
        return false;

    context.normal_ = NormalAt(context.point_);
    context.point_ = (*transform_)(context.point_);
    context.mat_ = mat_;

    return true;
}

BoundingBox Instance::BBox() const {
    if(transform_ == nullptr)
        return shape_->BBox();

    Point3D bbox_min = shape_->BBox().Min();
    Point3D bbox_max = shape_->BBox().Max();

    auto length_x = bbox_max[0] - bbox_min[0];
    auto length_y = bbox_max[1] - bbox_min[1];
    auto length_z = bbox_max[2] - bbox_min[2];

    Point3D transformed_bbox_min = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    Point3D transformed_bbox_max = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };
    for (int i = 0; i < 8; i++) {
        Point3D p { bbox_min[0] + ((i & (1 << 0)) >> 0) * length_x,
                    bbox_min[1] + ((i & (1 << 1)) >> 1) * length_y,
                    bbox_min[2] + ((i & (1 << 2)) >> 2) * length_z };

        Point3D transformed_p = (*transform_)(p);

        for(int j = 0; j < 3; j++) {
            transformed_bbox_min[j] = std::min(transformed_bbox_min[j], transformed_p[j]);
            transformed_bbox_max[j] = std::max(transformed_bbox_max[j], transformed_p[j]);
        }
    }

    return BoundingBox {transformed_bbox_min, transformed_bbox_max};
}
InstanceType Instance::Type() const {
    return instance_type_;
}



void Instance::GetTriangles(std::vector<Instance>& instances) const {
    if (instance_type_ != InstanceType::Mesh)
        throw std::logic_error("Can only get triangles from a mesh");

    auto mesh_ptr = dynamic_cast<Mesh*>(shape_.get());

    auto triangles = mesh_ptr->Triangles();
    for (auto& triangle : triangles) {
        auto triangle_ptr = std::make_shared<Triangle>(triangle);
        auto triangle_instance = Instance{transform_, triangle_ptr, mat_, InstanceType::Triangle};

        instances.push_back(triangle_instance);
    }
}