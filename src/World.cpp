//
// Created by Advil on 11/20/2024.
//

#include "../include/World.h"

void World::AddShape(std::shared_ptr<Shape> shape)
{
    world_.push_back(std::move(shape));
}

bool World::Hit(const Ray& r, ShadeContext& context)
{
    bool bHit = false;

    for(const std::shared_ptr<Shape>& shape_ptr : world_)
    {
        bHit |= shape_ptr->Hit(r, context);
    }

    return bHit;
}