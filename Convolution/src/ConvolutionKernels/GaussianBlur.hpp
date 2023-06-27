#include <opencv2/opencv.hpp>

class GaussianBlur : public Mask
{
    private:
        double sigma;
    public:
        GaussianBlur(int kernel_radius, double sigma);
        virtual ~GaussianBlur();
};