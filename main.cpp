#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

const std::vector< std::vector<int>> sobel_h_kernel = {{1,    0,      1}, {2,    0,      2}, {1,    0,      1}};
const std::vector< std::vector<int>> sobel_v_kernel = {{1,    2,      1}, {0,    0,      0}, {-1,  -2,     -1}};

const std::vector< std::vector<int>> sharpen_kernel = {{0,    -1,      0}, {-1,    5,      -1}, {0,  -1,     0}};

const std::vector< std::vector<int>> edge_kernel_weak = {{1,    0,      -1}, {0,    0,      0}, {-1,  0,     0}};
const std::vector< std::vector<int>> edge_kernel_normal = {{-1,    -1,      -1}, {-1,    8,      -1}, {-1,  -1,     -1}};
const std::vector< std::vector<int>> edge_kernel_strong = {{-1,    -1,      -1}, {-1,    8,      -1}, {-1,  -1,     -1}};

inline static uchar CheckBorderPixel(cv::Mat src, int x, int y, int ch) {
        if ((x < 0) || (x >= src.cols) || (y < 0) || (y >= src.rows)) {
                return 0;
        } else {
                return src.at<cv::Vec3b>(y, x)[ch];
        }
}
                                 
auto ImageConvolute(cv::Mat src, std::vector<std::vector<int>> kernel)
{
        cv::Mat outImage(src.rows, src.cols, CV_8UC3);
        
        
        for (int x = 0; x < src.cols ; x++)
        {
                for (int y = 0; y < src.rows; y++) {
                        cv::Vec3b accumulator(0, 0, 0);
                        
                        for (int kx = 0; kx < kernel.size(); kx++) {
                                for (int ky = 0; ky < kernel[kx].size(); ky++) {
                                       for(int ch = 0; ch < 3; ch++) {
                                               auto tmp = src.at<cv::Vec3b>(y, x)[ch];
                                               accumulator[ch] += CheckBorderPixel(src, x + kx, y + ky, ch) * kernel[kx][ky];
                                       }
                                        
                                }
                                
                        }
                        
                       for(int ch = 0; ch < 3; ch++) {
                               if(accumulator[ch] > 255)
                                       accumulator[ch] = 255;
                               else if (accumulator[ch] < 0)
                                       accumulator[ch] = 0;
                       }
                       outImage.at<cv::Vec3b>(y, x) = accumulator;
                       
                }
                
        }
        return outImage;
}

int main() {

        cv::namedWindow( "Test 1");
        auto src = cv::imread("TestData/lena.jpg");
        auto dst = src.clone();
        dst = ImageConvolute(src, edge_kernel_weak);
        cv::imshow("Test 1", dst);
        std::cout << dst.cols << std::endl;
        std::cout << dst.rows << std::endl;
        cv::waitKey(0);
        return 0;
}
