#include "../include/JitteredSampler.h"

std::vector<Point2D> JitteredSampler::Sample() {
    std::vector<Point2D> samples;

    int num_samples_per_dimension = std::ceil(std::sqrt(num_samples_));

    for(int i = 0; i < num_samples_per_dimension; i++) {
        for(int j = 0; j < num_samples_per_dimension; j++) {
            Point2D pixel_center { (i + 0.5) * (1.0 / num_samples_per_dimension), (j + 0.5) * (1.0 / num_samples_per_dimension)};

            Vec2D random_offset_x { RandomDouble(-0.5, 0.5) * (1.0 / num_samples_per_dimension), 0.0 };
            Vec2D random_offset_y {0.0, RandomDouble(-0.5, 0.5) * (1.0 / num_samples_per_dimension)};

            auto sample = pixel_center + random_offset_x + random_offset_y;

            samples.push_back(sample);
        }
    }

    return samples;
}