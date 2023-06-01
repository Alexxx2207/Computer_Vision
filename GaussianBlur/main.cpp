#include <iostream>
#include <opencv2/opencv.hpp>
#include "GaussianBlurMask.cpp"

void ImageBlur(cv::Mat workingImage, int size);

int main(int, char**) {
    cv::Mat sourceImage = cv::imread("../images/Lena.png", cv::IMREAD_COLOR);
    
    if (sourceImage.empty()) {
        std::cout << "Image not found!";
        return -1;
    }

    int size1 = 5;
    int size2 = 20;

    cv::Mat workingImage5;
    cv::Mat workingImage20;

    cv::imshow("Original Image", sourceImage);

    sourceImage.copyTo(workingImage5);
    ImageBlur(workingImage5, size1);
    cv::imshow("Blured Image - kernel 5X5", workingImage5);

    sourceImage.copyTo(workingImage20);
    ImageBlur(workingImage20, size2);
    cv::imshow("Blured Image - kernel 20X20", workingImage20);

    cv::waitKey();
}

void ImageBlur(cv::Mat workingImage, int size)
{
    GaussianBlur* gaussianBlur = new GaussianBlur(size);

    for (int row = 0; row < workingImage.rows - size; row++)
    {
        for (int col = 0; col < workingImage.cols - size; col++)
        {
            auto newPixel = cv::Vec3b();

            for (int convolutionPixel = 0; convolutionPixel < pow(gaussianBlur->size, 2); convolutionPixel++)
            {
                auto pixel = workingImage.at<cv::Vec3b>(
                    row + convolutionPixel / gaussianBlur->size,
                    col + convolutionPixel % gaussianBlur->size);

                for (int bgr = 0; bgr < 3; bgr++)
                {
                    newPixel[bgr] += static_cast<int>(round(pixel[bgr] * 
                        gaussianBlur->kernel[convolutionPixel / gaussianBlur->size][convolutionPixel % gaussianBlur->size]));
                }
            }
            
            workingImage.at<cv::Vec3b>(row + gaussianBlur->radius, col + gaussianBlur->radius) = newPixel;
        }
    }

    for (int borderIndexRow = 0; borderIndexRow < workingImage.rows; borderIndexRow++)
    {
        for (int borderIndexCol = 0; borderIndexCol < gaussianBlur->radius; borderIndexCol++)
        {
            workingImage.at<cv::Vec3b>(borderIndexRow, borderIndexCol) = 
                workingImage.at<cv::Vec3b>(borderIndexRow, gaussianBlur->radius);
            workingImage.at<cv::Vec3b>(borderIndexRow, workingImage.cols - borderIndexCol - 1) =
                workingImage.at<cv::Vec3b>(borderIndexRow, workingImage.cols - gaussianBlur->radius - 1);
        }
    }
    
    for (int borderIndexCol = 0; borderIndexCol < workingImage.cols; borderIndexCol++)
    {
        for (int borderIndexRow = 0; borderIndexRow < gaussianBlur->radius; borderIndexRow++)
        {
            workingImage.at<cv::Vec3b>(borderIndexRow, borderIndexCol) =
                workingImage.at<cv::Vec3b>(gaussianBlur->radius, borderIndexCol);
            workingImage.at<cv::Vec3b>(workingImage.rows - borderIndexRow - 1, borderIndexCol) = 
                workingImage.at<cv::Vec3b>(workingImage.rows - gaussianBlur->radius - 1, borderIndexCol);
        }
    }

    delete gaussianBlur;
}