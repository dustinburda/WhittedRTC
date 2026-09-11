//
// Created by Advil on 11/23/2024.
//

#include "../include/Triangle.h"

#include "../include/Util.h"

#include <cassert>
#include <sstream>
#include <string>

Normal<double, 3> Triangle::NormalAt(const Point<double, 3> &) const
{
    auto cross_product = Cross(vertices_[1].position_ - vertices_[0].position_,
                               vertices_[2].position_ - vertices_[0].position_);
    auto normalized_cross_product = cross_product.UnitVector();

    return Normal<double, 3> {normalized_cross_product[0], normalized_cross_product[1], normalized_cross_product[2]};
}

bool Triangle::Hit(const Ray &r, ShadeContext &context) const
{
    auto normal = NormalAt(vertices_[0].position_);
    if(std::abs(Dot(r.Direction(), normal)) <= epsilon)
        return false;

    double hit_time = Dot(normal, vertices_[0].position_ - r.Origin()) / Dot(normal, r.Direction());
    if (hit_time > context.t_max_ || hit_time < context.t_min_)
        return false;

    auto hit_point = r.At(hit_time);

    if(!InTriangle(hit_point))
        return false;

    context.t_max_ = hit_time;
    context.point_ = hit_point;
    context.normal_ = normal;

    if (is_textured_){
        auto barycentric_coordinates = BarycentricCoordinates(hit_point);
        auto uv_vector_ = barycentric_coordinates[0] * vertices_[0].texture_coordinate_.value().HomogenousPointToCartesianVector() +
                          barycentric_coordinates[1] * vertices_[1].texture_coordinate_.value().HomogenousPointToCartesianVector() +
                          barycentric_coordinates[2] * vertices_[2].texture_coordinate_.value().HomogenousPointToCartesianVector();

        context.uv_ = Point2D{uv_vector_[0], uv_vector_[1]};
    } else {
        context.uv_ = Point2D {-1, -1};
    }

    return true;
}

BoundingBox Triangle::BBox() const {
    Point3D min { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    Point3D max { std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min() };

    for (auto& vertex : vertices_) {
        for (int i = 0; i < 3; i++) {
            min[i] = std::min(min[i], vertex.position_[i]);
            max[i] = std::max(max[i], vertex.position_[i]);
        }
    }

    return BoundingBox {min, max};
}

void Triangle::SetIsTextured(bool is_textured) {
    is_textured_ = is_textured;
}

[[nodiscard]] bool Triangle::InTriangle(const Point<double, 3>& hit_point) const {
    auto coordinates = BarycentricCoordinates(hit_point);

    double l1 = coordinates[0];
    double l2 = coordinates[1];
    double l3 = coordinates[2];

    return (l1 >= 0 && l2 >= 0 && l3 >= 0 && std::abs(l1 + l2 + l3 - 1) < epsilon);
}

std::array<double, 3> Triangle::BarycentricCoordinates(const Point<double, 3>& hit_point) const {
    std::array<double, 3> barycentric_coordinates {0.0, 0.0, 0.0};

    double area_triangle = Cross(vertices_[1].position_ - vertices_[0].position_,
                                 vertices_[2].position_ - vertices_[0].position_).Length() / 2;

    for(int i = 0; i < 3; i++) {
        double area_sub_triangle = Cross(vertices_[(i + 1) % 3].position_ - vertices_[i % 3].position_,
                                         hit_point - vertices_[i % 3].position_).Length() / 2.0;
        barycentric_coordinates[(i + 2) % 3] = area_sub_triangle / area_triangle;
    }

    return barycentric_coordinates;
}

std::string Triangle::toString() const {
    std::stringstream ss;

    ss << "Triangle: [\n";

    ss << vertices_[0].position_.toString();
    ss << "\n";
    ss << vertices_[1].position_.toString();
    ss << "\n";
    ss << vertices_[2].position_.toString();
    ss << "\n";

    ss <<"]";

    return ss.str();
}

