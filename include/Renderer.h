//
// Created by Dustin on 7/4/26.
//

#ifndef WHITTED_RENDERER_H
#define WHITTED_RENDERER_H

#include "Color.h"
#include "ImagePlane.h"
#include "Material.h"
#include "SceneParser.h"
#include "ShadeContext.h"
#include "TileQueue.h"

#include <thread>
#include <vector>

class Renderer {
public:
    static Renderer& GetInstance();
    ~Renderer();

    void SetThreads(std::size_t num_threads);
    void Render(Scene& scene, Canvas& canvas) ;


private:
    Renderer();

    TileQueue GenerateTiles(const Canvas& canvas) const;
    void RenderTile(const Tile& t, Scene& scene, Canvas& canvas) const ;
    Color Trace(Scene& scene, Ray& r, ShadeContext& context) const ;

private:
    std::vector<std::thread> threads_;
    std::size_t num_threads_;
};

#endif //WHITTED_RENDERER_H
