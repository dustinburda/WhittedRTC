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
    auto&& tile_queue = GenerateTiles(canvas);

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

TileQueue Renderer::GenerateTiles( const Canvas& canvas) const {
    std::size_t width = canvas.Width();
    std::size_t height = canvas.Height();

    std::size_t tile_size = 32;

    TileQueue queue;
    std::size_t x_start = 0;
    while ( x_start < width) {
        std::size_t tile_x_length = std::min (width - x_start, tile_size);

        std::size_t y_start = 0;
        while (y_start < height) {
            size_t tile_y_length = std::min(height - y_start, tile_size);

            queue.AddTile({x_start, tile_x_length, y_start, tile_y_length});

            y_start += tile_y_length;
        }
        x_start += tile_x_length;
    }

    queue.Close();

    return queue;
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
