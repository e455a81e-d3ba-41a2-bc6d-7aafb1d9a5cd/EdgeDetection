#include <iostream>
#include <vector>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat ApplyEdgeDetection(cv::Mat& src, const std::vector<std::vector<int>>& hKernel, const std::vector<std::vector<int>>& vKernel);
cv::Mat ImageConvolute(cv::Mat& src, const std::vector<std::vector<int>>& kernel, const double multiplier);

const std::vector< std::vector<int>> sobel_h_kernel =            {{ 1,    0,   -1}, { 2,    0,   -2}, { 1,    0,   -1}};
const std::vector< std::vector<int>> sobel_v_kernel =            {{ 1,    2,    1}, { 0,    0,    0}, {-1,   -2,   -1}};
const std::vector< std::vector<int>> sharpen_kernel =            {{ 0,   -1,    0}, {-1,    5,   -1}, { 0,   -1,    0}};
const std::vector< std::vector<int>> edge_kernel_weak =          {{ 1,    0,   -1}, { 0,    0,    0}, {-1,    0,    0}};
const std::vector< std::vector<int>> edge_kernel_normal =        {{ 0,    1,    0}, { 1,   -4,    1}, { 0,    1,    0}};
const std::vector< std::vector<int>> edge_kernel_strong =        {{-1,   -1,   -1}, {-1,    8,   -1}, {-1,   -1,   -1}};
const std::vector< std::vector<int>> gaussian_blur =             {{ 1,    2,    1}, { 2,    4,    2}, { 1,    2,    1}};


inline static uchar CheckBorderGetPixel(cv::Mat src, int x, int y, int ch) {
        if ((x < 0) || (x >= src.cols) || (y < 0) || (y >= src.rows)) {
                return 0;
        } else {
                return src.at<cv::Vec3b>(y, x)[ch];
        }
}

cv::Mat ImageConvolute(cv::Mat& src, const std::vector<std::vector<int>>& kernel, const double multiplier = 1)
{
        cv::Mat outImage(src.rows, src.cols, CV_8UC3);
        int size = kernel.size() / 2;
        
        for (int x = 0; x < src.cols ; x++)
        {
                for (int y = 0; y < src.rows; y++) {
                        double accumulator[3] = {0};
                        
                        for (int kx = -size; kx <= size; kx++) {
                                for (int ky = -size; ky <= size; ky++) {
                                        
                                       for (int ch = 0; ch < 3; ch++) {
                                               //auto tmp = src.at<cv::Vec3b>(y, x)[ch];
                                               accumulator[ch] += multiplier * CheckBorderGetPixel(src, x + kx, y + ky, ch) * kernel[ky + size][kx + size];
                                       }
                                        
                                }
                                
                        }
                       cv::Vec3b pixel(0,0,0); 
                       for(int ch = 0; ch < 3; ch++) {
                               if(accumulator[ch] > 255)
                                       accumulator[ch] = 255;
                               else if (accumulator[ch] < 0)
                                       accumulator[ch] = 0;
                              pixel[ch] = static_cast<uchar>(accumulator[ch]); 
                       }
                       outImage.at<cv::Vec3b>(y, x) = pixel;
                       
                }
                
        }
        return outImage;
}

cv::Mat ApplyEdgeDetection(cv::Mat& src, const std::vector<std::vector<int>>& hKernel, const std::vector<std::vector<int>>& vKernel) 
{
        
        cv::Mat outImage(src.rows, src.cols, CV_8UC3);
        int size = hKernel.size() / 2; //TODO: catch kernels of non equal size
        
        for (int x = 0; x < src.cols ; x++)
        {
                for (int y = 0; y < src.rows; y++) {
                        double hAccumulator[3] = {0};
                        double vAccumulator[3] = {0};
                        
                        for (int kx = -size; kx <= size; kx++) {
                                for (int ky = -size; ky <= size; ky++) {
                                        
                                       for (int ch = 0; ch < 3; ch++) {
                                               //auto tmp = src.at<cv::Vec3b>(y, x)[ch];
                                               hAccumulator[ch] += CheckBorderGetPixel(src, x + kx, y + ky, ch) * hKernel[ky + size][kx + size];
                                               vAccumulator[ch] += CheckBorderGetPixel(src, x + kx, y + ky, ch) * vKernel[ky + size][kx + size];
                                       }
                                        
                                }
                                
                        }
                       cv::Vec3b pixel(0,0,0); 
                       for(int ch = 0; ch < 3; ch++) {
                               
                               auto tmp = sqrt((hAccumulator[ch] * hAccumulator[ch]) + (vAccumulator[ch] * vAccumulator[ch]));
                               
                               if(tmp > 255)
                                       tmp = 255;
                               else if (tmp < 0)
                                       tmp = 0;
                               
                              pixel[ch] = static_cast<uchar>(tmp); 
                       }
                       outImage.at<cv::Vec3b>(y, x) = pixel;
                       
                }
                
        }
        return outImage;
        
}

std::vector<std::vector<int>> CalculateLaplacianOfGaussianKernel(int size, double sigma = 1.4)
{
        std::vector<std::vector<int>> result(size, std::vector<int> (size, 0));
        int range = size / 2;
        
        int xcount = 0;
        for (int x = -range; x <= range; x++) {
                int ycount = 0;
                for (int y = -range; y <= range; y++) {
                        double tmp = (pow(x, 2) + pow(y, 2)) / (2.0 * pow(sigma, 2));
                        double val = -1.0 / (M_PI * pow(sigma, 4)) * (1.0 - tmp) * exp(-tmp);
                        result[xcount][ycount] = static_cast<int>(round(val*480));
                        ycount++;
                }
                xcount++;
        }
        
        return result;
}

int main() 
{
        auto test = CalculateLaplacianOfGaussianKernel(7, 1);
        /*std::for_each(test.begin(), test.end(), 
                [](std::vector<int>& it) {
                        std::for_each(it.begin(), it.end(), [](int& val) {
                                std::cout << val << " ";
                        });
                        std::cout << std::endl;
                }
        );*/
        
        cv::namedWindow( "Test 1");
        auto src = cv::imread("TestData/lena.jpg");
        auto dst = ImageConvolute(src, test);
        //dst = ApplyEdgeDetection(dst, sobel_h_kernel, sobel_v_kernel);
        cv::imshow("Test 1", dst);
        cv::waitKey(0);
        return 0;
}
