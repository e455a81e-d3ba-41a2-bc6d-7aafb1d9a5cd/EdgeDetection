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

#include "ImageProcessing.h"
#include <cmath>
#include <exception>

//If a kernel overlaps the border we assume a black pixel
inline static uchar CheckBorderGetPixel(cv::Mat src, int x, int y) {
    if ((x < 0) || (x >= src.cols) || (y < 0) || (y >= src.rows)) {
        return 0;
    } else {
        return src.at<uchar>(y, x);
    }
}

std::unique_ptr<cv::Mat> ImageConvolute(cv::Mat& src, const std::vector<std::vector<int>>& kernel, const double multiplier)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8U));
    int size = kernel.size() / 2;
    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            double accumulator = 0;

            for (int kx = -size; kx <= size; kx++) {
                for (int ky = -size; ky <= size; ky++) {

                    accumulator += multiplier * CheckBorderGetPixel(src, x + kx, y + ky) * kernel[ky + size][kx + size];

                }

            }
            //Thresholding is used to guarantee that all pixel values fit in one unsigned char
            if(accumulator > 255)
                accumulator = 255;
            else if (accumulator < 0)
                accumulator = 0;
            uchar pixel = accumulator; 
            outImage->at<uchar>(y, x) = pixel;

        }

    }
    return outImage;
}

std::unique_ptr<cv::Mat> ApplyEdgeDetection(cv::Mat& src, const std::vector<std::vector<int>>& hKernel, const std::vector<std::vector<int>>& vKernel) 
{

    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8U));
    int size = hKernel.size() / 2; //TODO: catch kernels of non equal size

    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            double hAccumulator = 0;
            double vAccumulator = 0;

            for (int kx = -size; kx <= size; kx++) {
                for (int ky = -size; ky <= size; ky++) {

                    hAccumulator += CheckBorderGetPixel(src, x + kx, y + ky) * hKernel[ky + size][kx + size];
                    vAccumulator += CheckBorderGetPixel(src, x + kx, y + ky) * vKernel[ky + size][kx + size];

                }

            }

            uchar pixel = 0;
            auto tmp = sqrt((hAccumulator * hAccumulator) + (vAccumulator * vAccumulator));

            if(tmp > 255)
                tmp = 255;
            else if (tmp < 0)
                tmp = 0;

            pixel = static_cast<uchar>(tmp); 
            outImage->at<uchar>(y, x) = pixel;

        }

    }
    return outImage;

}


std::unique_ptr<cv::Mat> ApplyRobertsEdgeDetection(cv::Mat& src) 
{
    const std::vector<std::vector<int>>& hKernel = roberts_h_kernel;
    const std::vector<std::vector<int>>& vKernel = roberts_v_kernel;

    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8UC1));

    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            double hAccumulator = 0;
            double vAccumulator = 0;

            for (int kx = 0; kx <= 1; kx++) {
                for (int ky = 0; ky <= 1; ky++) {

                    hAccumulator += CheckBorderGetPixel(src, x + kx, y + ky) * hKernel[ky][kx];
                    vAccumulator += CheckBorderGetPixel(src, x + kx, y + ky) * vKernel[ky][kx];

                }

            }
            uchar pixel(0); 

            auto tmp = sqrt((hAccumulator * hAccumulator) + (vAccumulator * vAccumulator)) * 2;

            if(tmp > 255)
                tmp = 255;
            else if (tmp < 0)
                tmp = 0;

            pixel = static_cast<uchar>(tmp); 
            outImage->at<uchar>(y, x) = pixel;

        }

    }
    return outImage;

}

std::vector<std::vector<int>> CalculateLaplacianOfGaussianKernel(int size, double sigma)
{
    if((size % 2) == 0) { std::runtime_error("Only even numbers are supported."); } //TODO: Richtige Exception Class schreiben

    std::vector<std::vector<int>> result(size, std::vector<int> (size, 0));

    double range = (static_cast<double>(size) - 1.0) / 4.0;

    int xcount = 0;
    for (double x = -range; x <= range; x += 0.5) {
        int ycount = 0;
        for (double y = -range; y <= range; y += 0.5) {
            double tmp = (pow(x, 2) + pow(y, 2)) / 2.0 * pow(sigma, 2);
            double val = -1.0 / (M_PI * pow(sigma, 4)) * (1.0 - tmp) * exp(-tmp);
            result[xcount][ycount] = static_cast<int>(ceil(val*480));
            ycount++;
        }
        xcount++;
    } 
    return result;

}

std::vector<std::vector<int>> CalculateGaussianKernel(int size, double sigma)
{
    std::vector<std::vector<int>> result(size, std::vector<int> (size, 0));
    int range = size / 2;

    int xcount = 0;
    for (int x = -range; x <= range; x++) {
        int ycount = 0;
        for (int y = -range; y <= range; y++) {
            double tmp = (pow(x, 2) + pow(y, 2)) / (2.0 * pow(sigma, 2));
            double val = (1.0 / (2* M_PI* pow(sigma, 2))) * exp(-tmp);
            result[xcount][ycount] = static_cast<int>(round(val*273));
            ycount++;
        }
        xcount++;
    }

    return result;

}

std::unique_ptr<cv::Mat> ThresholdImage(cv::Mat& src, int threshold)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8U));
    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            auto tmp = src.at<uchar>(y, x);
            if(tmp > threshold)
                tmp = 255;
            else
                tmp = 0;
            outImage->at<uchar>(y,x) = tmp;
        }


    }
    return outImage;

}


std::unique_ptr<cv::Mat> FindZeroCrossings(cv::Mat& src)
{

    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8U));
    for (int x = 0; x < src.cols ; x++) {
        for(int y = 0; y < src.rows; y++) {
            
            char n = CheckBorderGetPixel(src, y-1, x);
            char s = CheckBorderGetPixel(src, y+1, x);
            char nw = CheckBorderGetPixel(src, y-1, x-1);
            char se = CheckBorderGetPixel(src, y+1, x+1);
            char w = CheckBorderGetPixel(src, y, x-1);
            char e = CheckBorderGetPixel(src, y, x+1);
            
            if(n * s < 0){
                outImage->at<uchar>(x, y) = 255;
            } else if(nw * se < 0){
                outImage->at<uchar>(x, y) = 255;
            } else if(se*nw < 0){
                outImage->at<uchar>(x, y) = 255;
            } else if(w * e < 0){
                outImage->at<uchar>(x, y) = 255;
            } else {
                outImage->at<uchar>(x, y) = 0;
            }
        }
    }
    return outImage;
}
