//
// Created by Dustin on 12/1/24.
//

#ifndef INSTANCE_H
#define INSTANCE_H

#include "OBJParser.h"
#include "ShapeInterface.h"
#include "Transformation.h"

#include <memory>

enum class InstanceType {
    Sphere,
    Mesh,
    Triangle,
    Plane
};

class Instance : public ShapeInterface {
public:
    Instance() = delete;
    Instance(std::shared_ptr<ShapeInterface> shape, std::shared_ptr<MaterialInterface> mat, InstanceType type);
    Instance(std::shared_ptr<Transformation> t, std::shared_ptr<ShapeInterface> shape, std::shared_ptr<MaterialInterface> mat, InstanceType type);

    Normal<double, 3> NormalAt(const Point<double, 3>& p) const override;
    bool Hit(const Ray& r, ShadeContext& context) const override;
    BoundingBox BBox() const override;
    InstanceType Type() const;

    double SurfaceAreaBBox();

    void GetTriangles(std::vector<Instance>& instances) const;
    
private:
    BoundingBox ComputeBBox();

    std::shared_ptr<Transformation> transform_;
    std::shared_ptr<ShapeInterface> shape_;
    std::shared_ptr<MaterialInterface> mat_;
    BoundingBox bounding_box_;
    InstanceType instance_type_;
};



#endif //INSTANCE_H
