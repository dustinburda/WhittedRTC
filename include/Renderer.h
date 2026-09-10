//
// Created by Dustin on 7/4/26.
//

#ifndef WHITTED_RENDERER_H
#define WHITTED_RENDERER_H

#include "Color.h"
#include "ImagePlane.h"
#include "SceneParser.h"
#include "ShadeContext.h"
#include "Material.h"

#include <thread>
#include <vector>


class Renderer {
public:
    static Renderer& GetInstance() {
        static Renderer renderer;

        return renderer;
    }

    ~Renderer() {
        for (auto&& t : pool_)
            t.join();
    }

    /*
     * In: Scene to Render
     * Out: Canvas with rendered scene
     *
     */
    void Render (Scene& scene, Canvas& canvas) {

        auto trace = [](Scene& scene, Ray& r, ShadeContext& context) -> Color {
            if(scene.world_->Hit(r, context))
            {
                return context.mat_->Shade(context, scene.lights_);
            }

            return {0.0, 0.0, 0.0};
        };

        auto render = [&scene, &canvas, &trace](int x_start, int chunk_x_size, int y_start, int chunk_y_size) {
            ImagePlane p {canvas.Height(), canvas.Width()};

            for(int y = y_start; y < y_start + chunk_y_size; y++)
                for(int x = x_start; x < x_start + chunk_x_size; x++) {
                    auto ray = scene.camera_->GetRayAt(x, y, p);

                        ShadeContext context;
                        Color pixel_color = trace(scene, ray, context);

                    canvas.SetColorAt(pixel_color, x, y);
                }
        };

        std::size_t chunk_x_size = canvas.Width() / std::min<std::size_t>(canvas.Width(), num_threads_);
        std::size_t chunk_y_size = canvas.Height() /  std::min<std::size_t>(canvas.Height(), num_threads_);

        // TODO: Make a tile queue and fixed number of threads
        for (std::size_t y = 0; y < canvas.Height(); y += std::min(chunk_y_size, canvas.Height() - y))
            for(std::size_t x = 0; x < canvas.Width(); x += std::min(chunk_x_size, canvas.Width() - x)) {
                std::thread t ( render, x, chunk_x_size, y, chunk_y_size);
                pool_.push_back(std::move(t));
            }
    }


private:
    Renderer() : num_threads_{4} {

    }

    int num_threads_;
    std::vector<std::thread> pool_;
};

#endif //WHITTED_RENDERER_H
