/**  This file is part of EdgeDetection
 * 
 *  Copyright (C) 2014 Leonhardt Schwarz <if12b076@technikum-wien.at>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */


#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ImageProcessing.h"
#include "Debug.h"
#include <chrono>

#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 

auto src = cv::imread("TestData/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);

void CannyBench()
{
    for( int i = 0; i < 10; i++){
        INIT_TIMER
        START_TIMER
        (void)ApplyCanny(src, 35, 50);
        STOP_TIMER("Time")
    }
}

void SobelBench()
{
    
    for( int i = 0; i < 10; i++){
        INIT_TIMER
        START_TIMER
        (void)ApplyEdgeDetection(src, sobel_h_kernel, sobel_v_kernel);
        STOP_TIMER("Time")
    }
}

void PrewittBench()
{
    
    for( int i = 0; i < 10; i++){
        INIT_TIMER
        START_TIMER
        (void)ApplyEdgeDetection(src, prewitt_h_kernel, prewitt_v_kernel);
        STOP_TIMER("Time")
    }
}

void RobertsBench()
{
    for( int i = 0; i < 10; i++){
        INIT_TIMER
        START_TIMER
        (void)ApplyRobertsEdgeDetection(src);
        STOP_TIMER("Time")
    }
}

void CompassBench()
{
    
    for( int i = 0; i < 10; i++) {
        INIT_TIMER
        START_TIMER
        auto test = prewitt_h_kernel;
        std::vector<std::unique_ptr<cv::Mat>> compass(8);
        for(int i = 0; i < 8; i++)
        {
            compass[i] = ImageConvolute32S(src, test);
            if(i < 7) {
                test = RotateKernel(test, 45);
            }
        }
        
        std::vector<cv::Mat*> compass2(8);
        for(int i = 0; i < 8; i++)
        {
            compass2[i] = compass[i].get();
        }
        (void)FindHighestResponseValues(compass2);
        STOP_TIMER("Time")
    }
    
}

void LoGBench()
{
    for( int i = 0; i < 10; i++) {
        INIT_TIMER
        START_TIMER
        auto test = CalculateLaplacianOfGaussianKernel(7, 1.4);
        auto dst = ImageConvolute32S(src, test);
        (void)FindZeroCrossings(*dst, 100);
        STOP_TIMER("Time")
    }
    
}

int main() 
{
    
    cv::namedWindow( "Test 1");
    
    
    auto dst = ApplyCanny(src, 45,50);
    cv::imshow("Test 1", *dst);
    cv::waitKey(0);
    return 0;
}
