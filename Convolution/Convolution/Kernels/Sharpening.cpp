#include <cmath>
#include <opencv2/opencv.hpp>

#include "../Contracts/Mask.hpp"
#include "Sharpening.hpp"


Sharpening::Sharpening(int kernel_radius)
    : Mask(kernel_radius)
{
    for (int row = 0; row < this->kernel_width; row++)
    {
        for (int col = 0; col < this->kernel_width; col++)
        {
            this->kernel[this->kernel_width * row + col] = -1.0 / this->kernel_size;
        }
    }

    this->kernel[(this->kernel_width + 1) * this->kernel_radius] += 2;
}

Sharpening::~Sharpening() {}
