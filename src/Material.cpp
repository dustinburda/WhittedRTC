//
// Created by Advil on 11/20/2024.
//

#include "../include/Light.h"
#include "../include/Material.h"
#include "../include/ShadeContext.h"

MaterialInterface::MaterialInterface(MaterialType mat_type)
    : mat_type_{mat_type} {}

BlackMaterial::BlackMaterial()
    : MaterialInterface(MaterialType::Black) {}

Color BlackMaterial::Shade([[ maybe_unused ]] ShadeContext& context, [[ maybe_unused ]] std::vector<std::shared_ptr<Light>>& lights) {
    return {0.0, 0.0, 0.0};
}

SimplePhongMaterial::SimplePhongMaterial()
    : MaterialInterface{MaterialType::SimplePhong} {}

SimplePhongMaterial::SimplePhongMaterial(Color ka, Color kd, Color ks, double p)
        : MaterialInterface{MaterialType::SimplePhong}, ka_{ka}, kd_{kd}, ks_{ks}, p_{p} {}


Color SimplePhongMaterial::Shade(ShadeContext& context, std::vector<std::shared_ptr<Light>>& lights) {
    Color shade;

    // shade += ka_ * ambient_intensity;

    for (auto& light : lights)
    {
        auto l = light->GetDirection(context);
        auto n = context.normal_;

        auto n_dot_l = Dot(n, l);

        if (n_dot_l > 0.0) {
            auto light_color = light->GetIntensity() * light->GetColor();
            auto v = (-context.point_).HomogenousPointToCartesianVector().UnitVector(); // Camera is at origin


            auto h = (v + l).UnitVector();
            auto n_dot_h = std::max(0.0, Dot(n, h));
            shade += light_color * (kd_ * n_dot_l + ks_ * std::pow(n_dot_h, p_));
        }

//        auto r = Reflect(-l, n.ToVector());
//        auto r_dot_v = std::max(0.0, Dot(r, v));
//        shade += kd_ * light_color * n_dot_l + ks_ * light_color * std::pow(r_dot_v, p_);
    }

    return Clamp(shade);
}
