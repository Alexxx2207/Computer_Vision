#include <iostream>
#include <opencv2/opencv.hpp>

void menu(short& choice);
void rotateLeft(cv::Mat& image);
void rotateRight(cv::Mat& image);

int main(){
    cv::Mat image = cv::imread("../images/image.jpg", cv::IMREAD_COLOR);

    short choice = 0;
    std::string window_name = "Current Image";

    do
    {
        menu(choice);

        switch (choice)
        {
        case 0:
            cv::imshow(window_name, image);
            cv::waitKey();
            cv::destroyWindow(window_name); 
            break;
        case 1:
            rotateLeft(image);
            break;

        case 2:
            rotateRight(image);
            break;

        case 3:
            std::cout << std::endl << "Exit" << std::endl;
            break;

        default:
            std::cout << std::endl << "Didn't understand! Try again!" << std::endl;
            break;
        }

    } while (choice != 3);
}

void menu(short& choice)
{
    std::cout << std::endl << "Menu:" << std::endl;
    std::cout << "0. Open Image (Current State)" << std::endl;
    std::cout << "1. Rotate Left" << std::endl;
    std::cout << "2. Rotate Right" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << std::endl << "Choose rotation direction: ";
    std::cin >> choice;
}

void rotateLeft(cv::Mat& image)
{
    cv::Mat temp(cv::Size(image.rows, image.cols), CV_8UC3);

    for (int col = image.cols - 1, temp_row = 0; col >= 0; col--, temp_row++)
    {
        cv::Mat row_image = image.col(col);
        
        for (int temp_col = 0; temp_col < row_image.rows; temp_col++)
        {
            temp.at<cv::Vec3b>(temp_row, temp_col) = row_image.at<cv::Vec3b>(temp_col);
        }
    }

    image = temp;

    std::cout << std::endl << "Successful Left Rotation" << std::endl;
}

void rotateRight(cv::Mat& image)
{
    cv::Mat temp(cv::Size(image.rows, image.cols), CV_8UC3);

    for (int col = 0, temp_row = 0; col < image.cols; col++, temp_row++)
    {
        cv::Mat row_image = image.col(col);
        
        for (int temp_col = row_image.rows - 1; temp_col >= 0; temp_col--)
        {
            temp.at<cv::Vec3b>(temp_row, row_image.rows - 1 - temp_col) = row_image.at<cv::Vec3b>(temp_col);
        }
    }

    image = temp;

    std::cout << std::endl << "Successful Right Rotation" << std::endl;
}
