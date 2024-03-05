#include <opencv2/opencv.hpp>

class EdgeDetection : public Mask
{
public:
    EdgeDetection();
    virtual ~EdgeDetection();

    virtual void convolve(cv::Mat& image_src, cv::Mat& image_dest) override;
};