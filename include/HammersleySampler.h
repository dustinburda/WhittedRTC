#include "../include/Sampler.h"

class HammersleySampler : public Sampler
{
public:
    HammersleySampler(int num_samples) : Sampler{num_samples} {}
    std::vector<Point2D> Sample() override;
};

double phi(int i);