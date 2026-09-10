#include "../include/Sampler.h"
#include "../include/Util.h"

class JitteredSampler : public Sampler
{
public:
    JitteredSampler(int num_samples) : Sampler{num_samples} {}
    std::vector<Point2D> Sample() override;
};
