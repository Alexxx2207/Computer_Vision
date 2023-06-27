#include <opencv2/opencv.hpp>

class Mask
{
    protected:
        int kernel_radius;
        int kernel_width;
        int kernel_size;
        double* kernel;

    public:
        Mask(int kernel_radius);
        virtual ~Mask() = 0;

        virtual void convolve(cv::Mat& image_src, cv::Mat& image_dest);
};