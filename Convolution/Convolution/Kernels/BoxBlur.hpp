#include <opencv2/opencv.hpp>

class BoxBlur : public Mask
{
public:
    BoxBlur(int kernel_radius);
    virtual ~BoxBlur();
};