#include <cmath>
#include <opencv2/opencv.hpp>

#include "../Contracts/Mask.hpp"
#include "GaussianBlur.hpp"

#ifndef PI
    #define PI 3.141592
#endif


GaussianBlur::GaussianBlur(int kernel_radius, double sigma)
    : Mask(kernel_radius)
{
    this->sigma = sigma;
    double common_expression = 2.0 * this->sigma * this->sigma;

    double sum = 0.0;

    for (int row = -this->kernel_radius; row <= this->kernel_radius; row++)
    {
        for (int col = -this->kernel_radius; col <= this->kernel_radius; col++)
        {
            int row_index = this->kernel_width * (this->kernel_radius + row);
            int col_index = this->kernel_radius + col;

            this->kernel[row_index + col_index] =
                exp( -(row * row + col * col) / common_expression ) / ( PI * common_expression );

            sum += this->kernel[row_index + col_index];    
        }
    }

    for (int row = 0; row < this->kernel_width; row++)
    {
        for (int col = 0; col < this->kernel_width; col++)
        {
            this->kernel[this->kernel_width * row + col] /= sum;
        }
    }
}

GaussianBlur::~GaussianBlur() {}

