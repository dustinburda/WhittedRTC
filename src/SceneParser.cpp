//
// Created by Dustin on 11/25/24.
//
#include "../include/AmbientLight.h"
#include "../include/HammersleySampler.h"
#include "../include/Instance.h"
#include "../include/JitteredSampler.h"
#include "../include/Light.h"
#include "../include/OrthographicCamera.h"
#include "../include/PointLight.h"
#include "../include/ProjectiveCamera.h"
#include "../include/Mesh.h"
#include "../include/Plane.h"
#include "../include/RandomSampler.h"
#include "../include/RegularSampler.h"
#include "../include/Sampler.h"
#include "../include/SceneParser.h"
#include "../include/ShapeInterface.h"
#include "../include/Sphere.h"
#include "../include/Transformation.h"
#include "../include/Triangle.h"
#include "../include/Util.h"


#include <sstream>
#include <string>

// TODO: Add more error handling and throw exceptions
// TODO: Change the function

std::shared_ptr<Transformation> SceneParser::ParseRotation(std::unique_ptr<XMLNode>& node) {
    std::string axis = node->attributes_["axis"];
    double degrees = std::stod(node->attributes_["val"]);
    double radians = degrees * pi / 180.0;

    std::shared_ptr<Transformation> t = nullptr;
    if (axis == "x") {
        t = std::make_shared<Transformation>(Transformation::RotationX(radians));
    } else if (axis == "y") {
        t = std::make_shared<Transformation>(Transformation::RotationY(radians));
    } else if (axis == "z") {
        t = std::make_shared<Transformation>(Transformation::RotationZ(radians));
    }

    return t;
}

std::shared_ptr<Transformation> SceneParser::ParseReflection(std::unique_ptr<XMLNode>& node) {
    std::string axis = node->attributes_["axis"];

    std::shared_ptr<Transformation> t = nullptr;
    if (axis == "x") {
        t = std::make_shared<Transformation>(Transformation::ReflectionX());
    } else if (axis == "y") {
        t = std::make_shared<Transformation>(Transformation::ReflectionY());
    } else if (axis == "z") {
        t = std::make_shared<Transformation>(Transformation::ReflectionZ());
    }

    return t;
}

std::shared_ptr<Transformation> SceneParser::ParseTranslation(std::unique_ptr<XMLNode>& node) {
    double x = (node->attributes_.count("x") > 0.0) ? std::stod(node->attributes_["x"]) : 0.0;
    double y = (node->attributes_.count("y") > 0.0) ? std::stod(node->attributes_["y"]) : 0.0;
    double z = (node->attributes_.count("z") > 0.0) ? std::stod(node->attributes_["z"]) : 0.0;

    double x_translation = x;
    double y_translation = y;
    double z_translation = z;

    return std::make_shared<Transformation>(Transformation::Translation(x_translation, y_translation, z_translation));
}

std::shared_ptr<Transformation> SceneParser::ParseScale(std::unique_ptr<XMLNode>& node) {
    double x_scale = (node->attributes_.count("x") > 0) ? std::stod(node->attributes_["x"]) : 0.0;
    double y_scale = (node->attributes_.count("y") > 0) ? std::stod(node->attributes_["y"]) : 0.0;
    double z_scale = (node->attributes_.count("z") > 0) ? std::stod(node->attributes_["z"]) : 0.0;

    return std::make_shared<Transformation>(Transformation::Scale(x_scale, y_scale, z_scale));
}

std::shared_ptr<Transformation> SceneParser::ParseTransformation(std::unique_ptr<XMLNode>& node) {
    auto transformation = std::make_shared<Transformation>(Transformation::Identity());

    for (auto& child : node->children_) {
        if (child->tag_ == "rotation") {
            *transformation = *ParseRotation(child) * *transformation;
        } else if (child->tag_ == "reflection") {
            *transformation = *ParseReflection(child) * *transformation;
        } else if (child->tag_ == "translation") {
            *transformation = *ParseTranslation(child) * *transformation;
        } else if (child->tag_ == "scale") {
            *transformation = *ParseScale(child) * *transformation;
        }
    }

    return transformation;
}

// TODO: Get rid of this diabolically stupid function
Point3D SceneParser::ParseVertex (std::string point) {
    std::stringstream ss {point};
    std::string token;

    double x, y, z;

    std::getline(ss, token, ',');
    x = std::stod(token);

    std::getline(ss, token, ',');
    y = std::stod(token);

    std::getline(ss, token, ',');
    z = std::stod(token);

    return Point3D {x, y, z};
}

std::shared_ptr<Triangle> SceneParser::ParseTriangle(std::unique_ptr<XMLNode>& node) {
    auto vertex1 { ParseVertex(node->attributes_["point1"]) };
    auto vertex2 { ParseVertex(node->attributes_["point2"]) };
    auto vertex3 { ParseVertex(node->attributes_["point3"]) };

    return std::make_shared<Triangle>(vertex1, vertex2, vertex3);
}

std::shared_ptr<Instance> SceneParser::ParseShape(std::unique_ptr<XMLNode>& node) {
    std::string type = node->attributes_["type"];

    // Handle case where transformation is not provided
    std::string transformation_name = node->attributes_["transformation"];
    std::string material_name = node->attributes_["material"];

    auto material_ptr = parsing_context_.name_material_[material_name];
    auto transformation_ptr = parsing_context_.name_transformation_[transformation_name]; // Confusing, fix this
    std::shared_ptr<ShapeInterface> shape_ptr = nullptr;
    InstanceType instance_type;

    if (type == "sphere") {
        instance_type = InstanceType::Sphere;
        shape_ptr = std::make_shared<Sphere>();
    } else if (type == "plane") {
        instance_type = InstanceType::Plane;
        shape_ptr = std::make_shared<Plane>();
    } else if (type == "mesh") {
        instance_type = InstanceType::Mesh;

        std::string filename = node->attributes_["filename"];
        auto mesh_data = OBJParser::GetInstance().ParseOBJ( "../models/" + filename);

        shape_ptr = std::make_shared<Mesh>(mesh_data);
    } else if (type == "triangle") {
        instance_type = InstanceType::Triangle;
        shape_ptr = ParseTriangle(node);
    }

    return std::make_shared<Instance>(transformation_ptr, shape_ptr, material_ptr, instance_type);
}

Color SceneParser::ParseColor(const XMLNode* node) {
    if (node == nullptr)
        throw std::runtime_error("Color node must exist");

    double r, b, g;

    r = std::stod(node->attributes_.at("r"));
    g = std::stod(node->attributes_.at("g"));
    b = std::stod(node->attributes_.at("b"));

    return Color {r, g, b};
}

std::shared_ptr<MaterialInterface> SceneParser::ParseMaterial(std::unique_ptr<XMLNode>& node) {
    std::string type = node->attributes_["type"];

    std::shared_ptr<MaterialInterface> mat = nullptr;

    if (type == "simple_phong") {
        mat = std::make_shared<SimplePhongMaterial>(ParseColor(node->ChildNode("ka")),
                                                    ParseColor(node->ChildNode("kd")),
                                                    ParseColor(node->ChildNode("ks")),
                                                    std::stod(node->ChildNode("power")->value_));
    } else if (type == "solid") {
      mat = std::make_shared<SolidMaterial>(ParseColor(node->ChildNode("color")));
    } else {
        mat = std::make_shared<BlackMaterial>();
    }

    return mat;
}

std::shared_ptr<Sampler> SceneParser::ParseSampler(std::unique_ptr<XMLNode>& node) {
    // TODO: handle case where there is no sampler node

    auto sampler_type = node->attributes_["type"];
    auto name = node->attributes_["name"];

    auto samples_node = node->ChildNode("samples");
    if (samples_node == nullptr)
        throw std::runtime_error("sampler node must have a samples child");

    auto num_samples = std::stoi(samples_node->value_);

    std::shared_ptr<Sampler> sampler = nullptr;

    if (sampler_type == "hammersley") {
        sampler = std::make_shared<HammersleySampler>(num_samples);
    } else if (sampler_type == "jittered") {
        sampler = std::make_shared<JitteredSampler>(num_samples);
    } else if (sampler_type == "regular") {
        sampler = std::make_shared<RegularSampler>(num_samples);
    } else if (sampler_type == "random") {
        sampler = std::make_shared<RandomSampler>(num_samples);
    }

    return sampler;
}

template<typename T>
T Parse3D(const XMLNode* node) {
    auto x = std::stod(node->attributes_.at("x"));
    auto y = std::stod(node->attributes_.at("y"));
    auto z = std::stod(node->attributes_.at("z"));

    return T {x, y, z};
}

std::unique_ptr<CameraInterface> SceneParser::ParseCamera(std::unique_ptr<XMLNode>& node) {
    std::unique_ptr<CameraInterface> camera = nullptr;

    auto type = node->attributes_["type"];

    auto origin = Parse3D<Point3D>(node->ChildNode("origin"));
    auto lookat = Parse3D<Point3D>(node->ChildNode("lookat"));
    auto up = Parse3D<Vec3D>(node->ChildNode("up"));
    auto fov = std::stoi(node->ChildNode("fov")->value_);

    if (type == "projective") {
        camera = std::make_unique<ProjectiveCamera>(origin, lookat, up, fov);
    } else if (type == "orthographic") {
        camera = std::make_unique<OrthographicCamera>(origin, lookat, up);
    } else {
        throw std::logic_error ("Uknown Camera Type");
    }

    return camera;
}

std::shared_ptr<Light> SceneParser::ParsePointLight(std::unique_ptr<XMLNode>& node) {
    auto color_node = node->ChildNode("color");
    if (color_node == nullptr)
        throw std::runtime_error("point light must have a color child node");
    auto color = ParseColor(color_node);


    auto position_node = node->ChildNode("position");
    if (position_node == nullptr)
        throw std::runtime_error("point light must have a position child node");
    auto position = Parse3D<Point3D>( position_node ); // TODO: THIS IS BS, REDO SCHEMA HERE


    auto intensity_node = node->ChildNode("intensity");
    if (intensity_node == nullptr)
        throw std::runtime_error("point light must have an intensity");
    double intensity = std::stoi(intensity_node->value_);

    return std::make_shared<PointLight>(position, intensity, color);
}

std::shared_ptr<Light> SceneParser::ParseAmbientLight(std::unique_ptr<XMLNode>& node) {
    auto intensity = std::stod(node->ChildNode("intensity")->value_);
    auto color = Color {1.0, 1.0, 1.0};

    return std::make_shared<AmbientLight>(intensity, color);
}

std::shared_ptr<Light> SceneParser::ParseLight(std::unique_ptr<XMLNode>& node) {

    std::shared_ptr<Light> light = nullptr;
    if (node->attributes_["type"] == "point") {
        light = ParsePointLight(node);
    } else if (node->attributes_["type"] == "ambient") {
        light = ParseAmbientLight(node);
    }

    return light;
}

Scene SceneParser::ParseScene(std::string path) {
    std::ifstream file {path};
    auto file_size = std::filesystem::file_size(path);

    std::string src;
    src.resize(file_size);
    file.read(src.data(), file_size);

    auto node = xml_parser_.Parse(src);

    Scene scene;

    for (auto& child : node->children_) {
        if (child->tag_ == "transformation")
            parsing_context_.name_transformation_[child->attributes_["name"]] = ParseTransformation(child);
        else if (child->tag_ == "shape")
            scene.world_->AddShape(*ParseShape(child)); // TODO: Stop copying Instance
        else if (child->tag_ == "material")
            parsing_context_.name_material_[child->attributes_["name"]] = ParseMaterial(child);
        else if (child->tag_ == "antialiasing")
            scene.sampler_ = parsing_context_.name_sampler_[child->attributes_["sampler"]];
        else if (child->tag_ == "sampler")
            parsing_context_.name_sampler_[child->attributes_["name"]] = ParseSampler(child);
        else if (child->tag_ == "camera")
            scene.camera_ = ParseCamera(child);
        else if (child->tag_ == "light") {
            scene.lights_.push_back(ParseLight(child));
        }
    }

    return scene;
}

SceneParser::SceneParser() : xml_parser_{XMLParser::GetInstance()} {}