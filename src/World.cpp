//
// Created by Advil on 11/20/2024.
//

#include "../include/World.h"

void World::AddShape(Instance instance)
{
    if(instance.Type() == InstanceType::Mesh) {
        // adds individual triangles from Mesh class
        instance.GetTriangles(instances_);
    }
    else {
        instances_.push_back(instance);
    }
}

void World::Build() {
    bvh_.Build(instances_);
}

bool World::Hit(const Ray& r, ShadeContext& context)
{
    return bvh_.Hit(r, context);
}
