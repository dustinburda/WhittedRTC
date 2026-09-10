#include "../include/RegularSampler.h"

std::vector<Point2D> RegularSampler::Sample() {
    std::vector<Point2D> samples;

    int num_samples_per_dimension = (std::ceil(std::sqrt(num_samples_)) + 1); // Number of dimensions

    for(int i = 0; i < num_samples_per_dimension; i++) {
        for(int j = 0; j < num_samples_per_dimension; j++) {
            samples.push_back(Point2D {i * (1.0 / num_samples_per_dimension), j * (1.0 / num_samples_per_dimension)});
        }
    }

    return samples;
}