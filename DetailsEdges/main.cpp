#include <iostream>
#include <opencv2/opencv.hpp>

#include "GaussianBlurMask.cpp"

#define TRUE 1
#define FALSE 1

class EdgeMask {
    public:
        const int size = 7;
        const int radius = 3;
};

void FindingEdge(cv::Mat workingImage, cv::Mat sourceImage);

void ImageBlur(cv::Mat workingImage, int size);

int main(int, char**) {
    
    cv::Mat sourceImage = cv::imread("../images/Lena.png", cv::IMREAD_COLOR);

    if(sourceImage.empty() == TRUE)
    {
        std::cout << "File not found" << std::endl;
        return -1;
    }

    cv::Mat workingImage;

    EdgeMask mask;

    sourceImage.copyTo(workingImage);

    cv::imshow("Original Image", workingImage);

    ImageBlur(workingImage, mask.size);

    FindingEdge(workingImage, sourceImage);

    cv::imshow("Edged Image", workingImage);

    cv::waitKey();
}

void FindingEdge(cv::Mat workingImage, cv::Mat sourceImage)
{
    for (int row = 0; row < workingImage.rows; row++)
    {
        for (int col =  0; col < workingImage.cols; col++)
        {
            workingImage.at<cv::Vec3b>(row, col) = sourceImage.at<cv::Vec3b>(row, col) - workingImage.at<cv::Vec3b>(row, col);
        }
    }
}


void ImageBlur(cv::Mat workingImage, int size)
{
    GaussianBlur* gaussianBlur = new GaussianBlur(size);

    for (int row = 0; row < workingImage.rows - size; row++)
    {
        for (int col = 0; col < workingImage.cols - size; col++)
        {
            cv::Vec3b newPixel;

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