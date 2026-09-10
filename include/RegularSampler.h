#include "../include/Sampler.h"

class RegularSampler : public Sampler
{
public:
    RegularSampler(int num_samples) : Sampler{num_samples} {}
    std::vector<Point2D> Sample() override;
};
