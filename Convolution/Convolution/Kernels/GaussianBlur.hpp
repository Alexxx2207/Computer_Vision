#include <opencv2/opencv.hpp>

class GaussianBlur : public Mask
{
private:
    double sigma;
public:
    GaussianBlur(double sigma);
    virtual ~GaussianBlur();
};