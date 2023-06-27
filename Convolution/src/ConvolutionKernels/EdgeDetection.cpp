#include <cmath>
#include <opencv2/opencv.hpp>

#include "../Contracts/Mask.hpp"
#include "EdgeDetection.hpp"

#ifndef KERNEL_RADIUS
    #define KERNEL_RADIUS 2
#endif


EdgeDetection::EdgeDetection()
    : Mask(KERNEL_RADIUS)
{
    for (int row = 0; row < this->kernel_width; row++)
    {
        for (int col = 0; col < this->kernel_width; col++)
        {
            this->kernel[this->kernel_width * row + col] = -1.0;
        }
    }

    this->kernel[(this->kernel_width + 1) * this->kernel_radius] = this->kernel_size - 1;
}

EdgeDetection::~EdgeDetection() {}

void EdgeDetection::convolve(cv::Mat& image_src, cv::Mat& image_dest)
{
    int image_width = image_src.cols;
    int image_height = image_src.rows;

    #pragma omp parallel for collapse(2)
    for (int row = -kernel_radius; row <= image_height - kernel_radius - 1; row++)
    {
        for (int col = -kernel_radius; col <= image_width - kernel_radius - 1; col++)
        {
            double pixel_value = 0;
            
            for (int kernel_row = -kernel_radius; kernel_row <= this->kernel_radius; kernel_row++)
            {
                for (int kernel_col = -kernel_radius; kernel_col <= this->kernel_radius; kernel_col++)
                {
                    int kernel_row_index = std::clamp<int>(row + kernel_row, 0, image_height - 1);
                    int kernel_col_index = std::clamp<int>(col + kernel_col, 0, image_width - 1);

                    uchar current_pixel = image_src.at<uchar>(kernel_row_index, kernel_col_index);

                    pixel_value += current_pixel * this->kernel[
                        (this->kernel_radius + kernel_row) * this->kernel_width + 
                        (this->kernel_radius + kernel_col)];
                }
            }

            pixel_value = std::clamp<int>(pixel_value, 0, 255);
            
            image_dest.at<uchar>(row + this->kernel_radius, col + this->kernel_radius) = pixel_value;
        }
    }
}