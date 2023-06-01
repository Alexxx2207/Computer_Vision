#include <iostream>
#include <opencv2/opencv.hpp>

#define TRUE 1
#define FALSE 1

class SharpeningMask {
    public:
        const int size = 3;
        const int radius = 1;
        int kernel[3][3] =
        {
            { 0, -1,  0},
            {-1,  2, -1},
            { 0, -1,  0}
        };
};

void SharpeningImage(cv::Mat workingImage, cv::Mat sourceImage, SharpeningMask mask);

int main(int, char**) {
    
    cv::Mat sourceImage = cv::imread("../images/Nature.jpg", cv::IMREAD_COLOR);

    if(sourceImage.empty() == TRUE)
    {
        std::cout << "File not found" << std::endl;
        return -1;
    }

    cv::Mat workingImage;

    SharpeningMask mask;

    sourceImage.copyTo(workingImage);

    SharpeningImage(workingImage, sourceImage, mask);

    cv::imshow("Original Iamge", sourceImage);

    cv::imshow("Sharpened Image", workingImage);

    cv::waitKey();
}

void SharpeningImage(cv::Mat workingImage, cv::Mat sourceImage, SharpeningMask mask)
{
    for (int row = 0; row < workingImage.rows - mask.size; row++)
    {
        for (int col =  0; col < workingImage.cols - mask.size; col++)
        {
            cv::Vec3b newPixel;

            for (int convolutionPixelIndex = 0; convolutionPixelIndex < pow(mask.size, 2); convolutionPixelIndex++)
            {
                cv::Vec3b pixel = 
                    workingImage.at<cv::Vec3b>(row + convolutionPixelIndex / mask.size, col + convolutionPixelIndex % mask.size);

                for (int i = 0; i < 3; i++)
                {
                    int pixelColorProduct = pixel[i] *
                        mask.kernel[convolutionPixelIndex / mask.size][convolutionPixelIndex % mask.size];

                    if(pixelColorProduct + newPixel[i] >= 0 && pixelColorProduct + newPixel[i] <= 255)
                    {
                        newPixel[i] += pixelColorProduct; 
                    }
                    else if(pixelColorProduct + newPixel[i] < 0)
                    {
                        newPixel[i] = 0;
                    }
                    else if(pixelColorProduct + newPixel[i] > 255)
                    {
                        newPixel[i] = 255;
                    }
                }
            }
            
            workingImage.at<cv::Vec3b>(row + mask.radius, col + mask.radius) = newPixel;
        }
    }

    for (int row = 0; row < workingImage.rows; row++)
    {
        for (int col =  0; col < workingImage.cols; col++)
        {
            workingImage.at<cv::Vec3b>(row, col) += sourceImage.at<cv::Vec3b>(row, col);
        }
    }
}
