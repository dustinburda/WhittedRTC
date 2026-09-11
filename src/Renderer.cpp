//
// Created by Dustin on 7/4/26.
//

#include "../include/Renderer.h"

Renderer& Renderer::GetInstance() {
    static Renderer renderer;

    return renderer;
}

Renderer::Renderer() {
    threads_.reserve(4);
}

Renderer::~Renderer() {
    for (auto&& thread : threads_)
        if (thread.joinable())
            thread.join();
}

void Renderer::SetThreads(std::size_t num_threads) {
    threads_.reserve(num_threads);
}

void Renderer::Render( Scene& scene, Canvas& canvas) {
    auto tile_queue = GenerateTiles(scene);

    auto run_thread = [&tile_queue, &scene, &canvas, this]() {
        while (auto tile = tile_queue.NextTile()) {
            RenderTile(tile.value(), scene, canvas);
        }
    };

    auto num_threads = threads_.size();
    for (std::size_t i = 0; i < num_threads; i++) {
        threads_.emplace_back(std::thread(run_thread));
    }
}

TileQueue Renderer::GenerateTiles( [[ maybe_unused ]] const Scene& scene) const {
    return TileQueue {};
}

void Renderer::RenderTile(const Tile& t, Scene& scene, Canvas& canvas) const {
    ImagePlane p {canvas.Height(), canvas.Width()};

    for (std::size_t y = t.y_; y < t.y_ + t.y_length_; y++)
        for (std::size_t x = t.x_; x < t.x_ + t.x_length_; x++) {
            auto ray = scene.camera_->GetRayAt(x, y, p);

            ShadeContext context;
            Color pixel_color = Trace(scene, ray, context);

            canvas.SetColorAt(pixel_color, x, y);
        }
}

Color Renderer::Trace(Scene& scene, Ray& r, ShadeContext& context) const {
    if(scene.world_->Hit(r, context))
        return context.mat_->Shade(context, scene.lights_);

    return {0.0, 0.0, 0.0};
}
