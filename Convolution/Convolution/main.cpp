#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "Contracts/Mask.hpp"
#include "Kernels/BoxBlur.hpp"
#include "Kernels/GaussianBlur.hpp"
#include "Kernels/Sharpening.hpp"
#include "Kernels/EdgeDetection.hpp"


#ifndef SOURCE_IMAGE_WINDOW_NAME
#define SOURCE_IMAGE_WINDOW_NAME "Source Image"
#endif

#ifndef COLORED_IMAGE_WINDOW_NAME
#define COLORED_IMAGE_WINDOW_NAME "Colored Image State"
#endif

#ifndef GRAYSCALED_IMAGE_WINDOW_NAME
#define GRAYSCALED_IMAGE_WINDOW_NAME "Grayscaled Image State"
#endif

#ifndef SIGMA_PARAMETER_NAME
#define SIGMA_PARAMETER_NAME "sigma"
#endif

#ifndef KERNEL_RADIUS_PARAMETER_NAME
#define KERNEL_RADIUS_PARAMETER_NAME "kernel radius"
#endif

#ifndef MENU_ACTIONS_COUNT
#define MENU_ACTIONS_COUNT 5
#endif


/// @brief Loads an image by a given file path with three channels and in grayscale
/// @param image_path Image file path to load 
/// @param colored_image_src Colored image matrix that will be read-only
/// @param colored_image_dest Colored image matrix that will be manipulated
/// @param grayscaled_image_dest Grayscaled image matrix that will be manipulated
/// @return True if image loading was successful, otherwise - False
bool loadImage(
    const std::string& image_path,
    cv::Mat& colored_image_src,
    cv::Mat& colored_image_dest,
    cv::Mat& grayscaled_image_dest);

/// @brief Shows image result after convolution process
void showImage(cv::Mat& colored_image_src, cv::Mat& colored_image_dest);

/// @brief Shows a menu of possible actions that can be done
void showMenu();

/// @brief Detects edges of a grayscale image. Edges are in white. A gaussian blur precedes the edge detection process.
/// @param grayscaled_image_src Grayscaled image matrix that will be read-only
/// @param grayscaled_image_dest Grayscaled image matrix that will be manipulated
void detectEdges(cv::Mat& grayscaled_image_src, cv::Mat& grayscaled_image_dest);

/// @brief Encapsulates the logic of requesting, handling input of, validating and saving a parameter value that will be used for a mask initialization
/// @param T Type of the parameter
/// @param parameter Reference to the parameter
/// @param parameter_name Name of the parameter (e.g. "Sigma", "Kernel radius")
/// @param minimal_value Minimal parameter value boundary - for input validation
template <typename T>           
void showParameterValueMenu(T& parameter, const std::string& parameter_name, double minimal_value);

int main() {
    cv::Mat colored_image_src, colored_image_dest, grayscaled_image_dest;

    bool sucessful_load = loadImage(
        "Assets/Sofia.png",
        colored_image_src,
        colored_image_dest,
        grayscaled_image_dest);

    if (!sucessful_load) {
        std::cout << "Image not found!";
        return -1;
    }

    short choice = 0;

    int kernel_radius = 1;
    double sigma = 1.0;

    Mask* mask = nullptr;

    while (choice != MENU_ACTIONS_COUNT) {
        showMenu();

        std::cout << std::endl << "-> Choose action: ";

        std::cin >> choice;

        switch (choice) {
        case 1:
            detectEdges(colored_image_src, grayscaled_image_dest);

            cv::imshow(SOURCE_IMAGE_WINDOW_NAME, colored_image_src);
            cv::imshow(GRAYSCALED_IMAGE_WINDOW_NAME, grayscaled_image_dest);

            cv::waitKey();
            cv::destroyAllWindows();

            break;
        case 2:
            showParameterValueMenu<int>(kernel_radius, KERNEL_RADIUS_PARAMETER_NAME, 0);

            mask = new BoxBlur(kernel_radius);

            mask->convolve(colored_image_src, colored_image_dest);

            delete mask;

            showImage(colored_image_src, colored_image_dest);
            break;
        case 3:
            showParameterValueMenu<double>(sigma, SIGMA_PARAMETER_NAME, 0);

            mask = new GaussianBlur( sigma);

            mask->convolve(colored_image_src, colored_image_dest);

            delete mask;

            showImage(colored_image_src, colored_image_dest);
            break;
        case 4:
            showParameterValueMenu<int>(kernel_radius, KERNEL_RADIUS_PARAMETER_NAME, 0);

            mask = new Sharpening(kernel_radius);

            mask->convolve(colored_image_src, colored_image_dest);

            delete mask;

            showImage(colored_image_src, colored_image_dest);
            break;
        case MENU_ACTIONS_COUNT:
            std::cout << std::endl << "Exiting..." << std::endl;
            break;
        default:
            std::cout << std::endl << "No action associated with your choice. Try again!" << std::endl;
            break;
        }
    }
}

bool loadImage(
    const std::string& image_path,
    cv::Mat& colored_image_src,
    cv::Mat& colored_image_dest,
    cv::Mat& grayscaled_image_dest) {
    colored_image_src = cv::imread(image_path.data(), cv::IMREAD_COLOR);

    if (colored_image_src.empty()) {
        return false;
    }

    colored_image_src.copyTo(colored_image_dest);

    grayscaled_image_dest =
        cv::Mat::zeros(colored_image_src.rows, colored_image_src.cols, CV_8UC1);

    return true;
}

void detectEdges(cv::Mat& colored_image_src, cv::Mat& grayscaled_image_dest) {
    cv::Mat temp_colored;
    cv::Mat temp_grayscaled =
        cv::Mat::zeros(colored_image_src.rows, colored_image_src.cols, CV_8UC1);

    colored_image_src.copyTo(temp_colored);

    Mask* mask = new GaussianBlur(0.8);

    mask->convolve(colored_image_src, temp_colored);

    delete mask;

    cv::cvtColor(temp_colored, temp_grayscaled, cv::COLOR_BGR2GRAY);

    mask = new EdgeDetection();

    mask->convolve(temp_grayscaled, grayscaled_image_dest);

    delete mask;
}

void showImage(cv::Mat& colored_image_src, cv::Mat& colored_image_dest) {
    cv::imshow(SOURCE_IMAGE_WINDOW_NAME, colored_image_src);
    cv::imshow(COLORED_IMAGE_WINDOW_NAME, colored_image_dest);

    cv::waitKey();
    cv::destroyAllWindows();
}

void showMenu() {
    std::cout << std::endl
        << "Menu (filter is always applied on the original image state):" << std::endl
        << "1. View Image's Edges (Edge Detection)" << std::endl
        << "2. Smooth Image (Box Blur)" << std::endl
        << "3. Smooth Image (Gaussian Blur)" << std::endl
        << "4. Sharpen Image" << std::endl
        << "5. Exit" << std::endl;
}

template <typename T>
void showParameterValueMenu(T& parameter, const std::string& parameter_name, double minimal_value) {
    do {
        std::cout << std::endl
            << "-> Choose " << parameter_name << " (> " << minimal_value << "): ";

        std::cin >> parameter;

        if (parameter <= minimal_value) {
            std::cout << std::endl << "Invalid " << parameter_name << std::endl;
        }
    } while (parameter <= minimal_value);
}