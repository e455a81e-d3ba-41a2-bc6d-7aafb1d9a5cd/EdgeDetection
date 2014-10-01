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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ImageProcessing.h"
#include "Debug.h"

int main() 
{
        auto test = CalculateGaussianKernel(5);
        printKernel(test);
        
        cv::namedWindow( "Test 1");
        
        auto src = cv::imread("TestData/lena.jpg");
        auto dst = ImageConvolute(src, test, 1.0/273.0);
        dst = ApplyEdgeDetection(*dst, sobel_h_kernel, sobel_v_kernel);
        //dst = ThresholdImage(*dst, 0);
        cv::imshow("Test 1", *dst);
        cv::waitKey(0);
        return 0;
}
