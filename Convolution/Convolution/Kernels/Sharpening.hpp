#include <opencv2/opencv.hpp>

class Sharpening : public Mask
{
public:
    Sharpening(int kernel_radius);
    virtual ~Sharpening();
};