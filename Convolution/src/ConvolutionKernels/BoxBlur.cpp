#include <algorithm>
#include <opencv2/opencv.hpp>

#include "../Contracts/Mask.hpp"
#include "BoxBlur.hpp"

BoxBlur::BoxBlur(int kernel_radius)
    : Mask(kernel_radius)
{
    for (int row = 0; row < this->kernel_width; row++)
    {
        for (int col = 0; col < this->kernel_width; col++)
        {
            this->kernel[this->kernel_width * row + col] = 1.0 / this->kernel_size;
        }
    }
}

BoxBlur::~BoxBlur() {}
