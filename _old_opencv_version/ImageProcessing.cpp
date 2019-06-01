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
#include <stack>
#include <utility>
#include "Debug.h"

std::unique_ptr<cv::Mat> ImageConvolute8U(cv::Mat& src, const std::vector<std::vector<int>>& kernel, const double multiplier)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8U));
    int size = kernel.size() / 2;
    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            double accumulator = 0;

            for (int kx = -size; kx <= size; kx++) {
                for (int ky = -size; ky <= size; ky++) {

                    accumulator += multiplier * CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * kernel[ky + size][kx + size];

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

                    hAccumulator += CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * hKernel[ky + size][kx + size];
                    vAccumulator += CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * vKernel[ky + size][kx + size];

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

                    hAccumulator += CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * hKernel[ky][kx];
                    vAccumulator += CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * vKernel[ky][kx];

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




std::unique_ptr<cv::Mat> ImageConvolute32S(cv::Mat& src, const std::vector<std::vector<int>>& kernel)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_32SC1));
    auto test = *outImage.get();
    int size = kernel.size() / 2;
    
    for (int x = 0; x < src.cols ; x++)
    {
        for (int y = 0; y < src.rows; y++) {
            int accumulator = 0;

            for (int kx = -size; kx <= size; kx++) {
                for (int ky = -size; ky <= size; ky++) {

                    accumulator += CheckBorderGetPixel<uchar>(src, x + kx, y + ky) * kernel[ky + size][kx + size];

                }

            }
            int pixel = accumulator; 
            outImage->at<int>(y, x) = pixel;

        }

    }
    return outImage;
}

std::unique_ptr<cv::Mat> FindZeroCrossings(cv::Mat& src, int threshold)
{

    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8UC1));
    
    for (int x = 0; x < src.cols ; x++) {
        for(int y = 0; y < src.rows; y++) {
            
            int n = CheckBorderGetPixel<int>(src, x, y-1);
            int s = CheckBorderGetPixel<int>(src, x, y+1);
            int nw = CheckBorderGetPixel<int>(src, x-1, y-1);
            int se = CheckBorderGetPixel<int>(src, x+1, y+1);
            int w = CheckBorderGetPixel<int>(src, x-1, y);
            int e = CheckBorderGetPixel<int>(src, x+1, y);
            int ne = CheckBorderGetPixel<int>(src, x+1,y-1);
            int sw = CheckBorderGetPixel<int>(src, x-1, y+1);
            
            if(n * s < 0){
                if (abs(n) + abs(s) > threshold)
                    outImage->at<uchar>(y, x) = 255;
            } else if(nw * se < 0){
                if (abs(nw) + abs(se) > threshold)
                    outImage->at<uchar>(y, x) = 255;
            } else if(sw*ne < 0){
                if (abs(sw) + abs(ne) > threshold)
                    outImage->at<uchar>(y, x) = 255;
            } else if(w * e < 0){
                if (abs(w) + abs(e) > threshold)
                    outImage->at<uchar>(y, x) = 255;
            } else {
                outImage->at<uchar>(y, x) = 0;
            }
        }
    }
    return outImage;
}

std::vector< std::vector<int>> RotateKernel(const std::vector<std::vector<int>>& kernel, int degree)
{
    std::vector<std::vector<int>> RotatedKernel(kernel.size(), std::vector<int>(kernel.front().size()));
    int xOffset = kernel.size() / 2;
    int yOffset = kernel.front().size() / 2;
    
    double rad = degree * M_PI / 180.0;
    
    for (int y = 0; y < kernel.size(); y++) {
        for(int x = 0; x < kernel.front().size(); x++) {
            
            int xCoord = round((x - xOffset) * cos(rad) - (y - yOffset) * sin(rad) + xOffset);
            int yCoord = round((x - xOffset) * sin(rad) +(y - yOffset) * cos(rad) + yOffset);
            RotatedKernel[yCoord][xCoord] = kernel[y][x];
        }
        
    }
    return RotatedKernel;
}

std::unique_ptr<cv::Mat> FindHighestResponseValues(std::vector<cv::Mat*> images)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(images.front()->rows, images.front()->cols, CV_8UC1));
    
    for (int x = 0; x < images.front()->cols; x++) {
        for(int y = 0; y < images.front()->rows; y++) {
            int highest = 0;
            for(int i = 0; i < images.size(); i++) {
                if (highest < images[i]->at<int>(y,x))
                    highest = images[i]->at<int>(y,x);
            }
            
            uchar pixel = highest; 
            if(highest > 255)
                pixel = 255;
            else if (highest < 0)
                pixel = 0;
            outImage->at<uchar>(y, x) = pixel;
        }
        
    }
    return outImage;
    
}

std::unique_ptr<cv::Mat> ApplyCanny(cv::Mat& src, int thresholdMax, int thresholdMin)
{
    auto gauss = CalculateGaussianKernel(5, 2.0);
    auto smooth = ImageConvolute8U(src, gauss, 1.0/273.0);
    auto Gh = ImageConvolute8U(*smooth, sobel_h_kernel);
    auto Gv = ImageConvolute8U(*smooth, sobel_v_kernel);
    auto MagnitudeImage = ApplyEdgeDetection(*smooth, sobel_h_kernel, sobel_v_kernel);
    
    auto NonMaximaSuppressionImage = NonMaximaSuppression(*MagnitudeImage, *Gh, *Gv);
    return TraceEdgesHysteresis(*NonMaximaSuppressionImage,thresholdMax, thresholdMin);
}

static inline double toDegrees(double theta)
{
    return (theta * 180)/180;
}

std::unique_ptr<cv::Mat> NonMaximaSuppression(cv::Mat& src, cv::Mat& Gh, cv::Mat& Gv)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(Gh));
    auto sector = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8UC1, cvScalar(0)));
    auto theta = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_64FC1, cvScalar(0)));
    
    //Calculate Gradient orientation
    for (int x = 0; x < src.cols ; x++) {
        for (int y = 0; y < src.rows; y++) {
            
            double tmp = atan2(Gh.at<uchar>(y,x), Gv.at<uchar>(y,x));
            theta->at<double>(y,x) = tmp;
        }
        
    }
    
    //Determine Sectors
    for (int x = 0; x < src.cols ; x++) {
        for (int y = 0; y < src.rows; y++) {
            double angle = toDegrees(theta->at<double>(y,x));
            //angle += 270;
            angle = fmod(angle, 360);
            
            if ((angle >= 337.5) || (angle < 22.5) || ((angle >= 157.5) && (angle < 202.5))) {
                sector->at<uchar>(y,x) = 0;
            } else if (((angle >= 22.5) && (angle < 67.5)) || ((angle >= 202.5) && (angle < 247.5))) {
                sector->at<uchar>(y,x) = 1;
            } else if (((angle >=67.5) && (angle < 112.5)) || ((angle >= 247.5) && (angle < 292.5))) {
                sector->at<uchar>(y,x) = 2;
            } else if (((angle >= 112.5) && (angle < 157.5)) || ((angle >=292.5) && (angle < 337.5))) {
                sector->at<uchar>(y,x) = 3;
            } else {
                sector->at<uchar>(y,x) = 0;
            }
            
        }
        
    }
    
    //Compare Magnitudes to the neighbours
    for (int x = 0; x < src.cols ; x++) {
        for (int y = 0; y < src.rows; y++) {
            uchar tmp = src.at<uchar>(y,x);
            switch(sector->at<uchar>(y,x)) {
                case 0:
                    if ((CheckBorderGetPixel<uchar>(src, x+1, y) >= tmp) || (CheckBorderGetPixel<uchar>(src, x-1, y) > tmp)) {
                        outImage->at<uchar>(y,x) = 0;
                    } else {
                        outImage->at<uchar>(y,x) = tmp;
                    }
                    break;
                case 1:
                    if ((CheckBorderGetPixel<uchar>(src, x+1, y+1) >= tmp) || (CheckBorderGetPixel<uchar>(src, x-1, y-1) > tmp)) {
                        outImage->at<uchar>(y,x) = 0;
                    } else {
                        outImage->at<uchar>(y,x) = tmp;
                    }
                    break;
                case 2:
                    if ((CheckBorderGetPixel<uchar>(src, x, y+1) >= tmp) || (CheckBorderGetPixel<uchar>(src, x, y-1) > tmp)) {
                        outImage->at<uchar>(y,x) = 0;
                    } else {
                        outImage->at<uchar>(y,x) = tmp;
                    }
                    break;
                case 3:
                    if ((CheckBorderGetPixel<uchar>(src, x+1, y-1) >= tmp) || (CheckBorderGetPixel<uchar>(src, x-1, y+1) > tmp)) {
                        outImage->at<uchar>(y,x) = 0;
                    } else {
                        outImage->at<uchar>(y,x) = tmp;
                    }
                    break;
            }
        }
        
    }
    return outImage;
}

std::unique_ptr<cv::Mat> TraceEdgesHysteresis(cv::Mat& src, int thresholdMax, int thresholdMin)
{
    auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat(src.rows, src.cols, CV_8UC1, cvScalar(0)));
    std::stack<std::pair<int, int>> edgeStack;
    
    for (int x = 0; x < src.cols ; x++) {
        for (int y = 0; y < src.rows; y++) {
            
            if((src.at<uchar>(y,x) >= thresholdMax) && (outImage->at<uchar>(y,x) == 0)) {
                //Strong pixel found push to stack
                outImage->at<uchar>(y,x) = 255;
                edgeStack.push(std::pair<int,int>(y,x));
                
                while(edgeStack.size() > 0) {
                    auto cur = edgeStack.top();
                    edgeStack.pop();
                    
                    //Surrounding pixel coordinates
                    const std::pair<int,int> nbs[8] {
                        std::pair<int,int>( cur.first-1, cur.second),
                        std::pair<int,int>( cur.first+1, cur.second),
                        std::pair<int,int>( cur.first-1, cur.second-1),
                        std::pair<int,int>( cur.first+1, cur.second+1),
                        std::pair<int,int>( cur.first, cur.second-1),
                        std::pair<int,int>( cur.first, cur.second+1),
                        std::pair<int,int>( cur.first-1, cur.second+1),
                        std::pair<int,int>( cur.first+1, cur.second-1)
                    };
                    
                    for(int i = 0; i < 8; i++) {
                        
                        if (!((nbs[i].second < 0) || (nbs[i].second >= src.cols) || 
                            (nbs[i].first < 0) || (nbs[i].first >= src.rows))) {
                            
                            if ((src.at<uchar>(nbs[i].first,nbs[i].second) >= thresholdMin) 
                                && (outImage->at<uchar>(nbs[i].first,nbs[i].second) == 0)) {
                                
                                outImage->at<uchar>(nbs[i].first, nbs[i].second) = 255;
                                edgeStack.push(nbs[i]);
                            }
                        
                        }
                    }
                    
                }
                    
            }
            
        }
    }
    return outImage;
}