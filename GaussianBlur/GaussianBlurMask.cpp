#include <cmath> 
#include <vector>

#define MATH_PI 3.14159265359

class GaussianBlur
{
    public:
        int size;
        int radius;
        double sigma = 0.5;
        std::vector<std::vector<double>> kernel;

        GaussianBlur(int size)
        {
            kernel.resize(size, std::vector<double>(size, 0));

            this->size = size;
            this->radius = size / 2;
            double kernelSum = 0;

            for (int x = -radius; x < radius; x++)
            {
                for (int y = -radius; y < radius; y++)
                {
                    kernel[x+radius][y+radius] = (exp(( -(x * x + y * y) / 2 * pow(sigma, 2)))) / (2 * MATH_PI * sigma);
                    kernelSum += kernel[x+radius][y+radius];
                }
            }

            for (int x = 0; x < size; x++)
            {
                for (int y = 0; y < size; y++)
                {
                    kernel[x][y] /= kernelSum;
                }
            }
        };

        ~GaussianBlur() {};
};
