#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ImageProcessing.h"
#include "Debug.h"

int main() 
{
        auto test = CalculateGaussianKernel(5, 1);
        printKernel(test);
        
        cv::namedWindow( "Test 1");
        
        auto src = cv::imread("TestData/lena.jpg");
        auto dst = ImageConvolute(src, test, 1.0/273.0);
        dst = ApplyEdgeDetection(dst, sobel_h_kernel, sobel_v_kernel);
        cv::imshow("Test 1", dst);
        cv::waitKey(0);
        return 0;
}
