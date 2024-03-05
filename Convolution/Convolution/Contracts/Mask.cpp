#include <omp.h>

#include "Mask.hpp"
#include "../Utils/MathFunctions.hpp"

Mask::Mask(int kernel_radius)
{
    this->kernel_radius = kernel_radius;
    this->kernel_width = kernel_radius * 2 + 1;
    this->kernel_size = kernel_width * kernel_width;
    delete[] kernel;
    this->kernel = new double[kernel_size];
}

Mask::~Mask()
{
    delete[] kernel;
}

void Mask::convolve(cv::Mat& image_src, cv::Mat& image_dest)
{
    MathFunctions mathFunctions = MathFunctions();
    int image_width = image_src.cols;
    int image_height = image_src.rows;

    #pragma omp parallel for collapse(2)
    for (int row = -this->kernel_radius; row <= image_height - this->kernel_radius - 1; row++)
    {
        for (int col = -this->kernel_radius; col <= image_width - this->kernel_radius - 1; col++)
        {
            cv::Vec3b pixel_result = cv::Vec3b();

            double channels_values[3] = { 0, 0, 0 }; /* {B, G, R} */

            for (int kernel_row = -this->kernel_radius; kernel_row <= this->kernel_radius; kernel_row++)
            {
                for (int kernel_col = -this->kernel_radius; kernel_col <= this->kernel_radius; kernel_col++)
                {
                    int kernel_row_index = mathFunctions.clamp(row + kernel_row, 0, image_height - 1);
                    int kernel_col_index = mathFunctions.clamp(col + kernel_col, 0, image_width - 1);

                    cv::Vec3b current_pixel = image_src.at<cv::Vec3b>(kernel_row_index, kernel_col_index);

                    for (short bgr = 0; bgr < 3; bgr++)
                    {
                        channels_values[bgr] += current_pixel[bgr] * this->kernel[
                            (this->kernel_radius + kernel_row) * this->kernel_width +
                                (this->kernel_radius + kernel_col)];
                    }
                }
            }

            for (short bgr = 0; bgr < 3; bgr++)
            {
                pixel_result[bgr] = mathFunctions.clamp((int)channels_values[bgr], 0, 255);
            }

            image_dest.at<cv::Vec3b>(row + this->kernel_radius, col + this->kernel_radius) = pixel_result;
        }
    }
}