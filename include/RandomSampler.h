#include "../include/Sampler.h"
#include "../include/Util.h"


class RandomSampler : public Sampler
{
public:
    RandomSampler(int num_samples) : Sampler{num_samples} {}
    std::vector<Point2D> Sample() override;
};