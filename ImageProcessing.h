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

#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <vector>
#include <memory>
#include "opencv2/imgproc/imgproc.hpp"

const std::vector< std::vector<int>> sobel_h_kernel =            {{ 1,    0,   -1}, { 2,    0,   -2}, { 1,    0,   -1}};
const std::vector< std::vector<int>> sobel_v_kernel =            {{ 1,    2,    1}, { 0,    0,    0}, {-1,   -2,   -1}};
const std::vector< std::vector<int>> sharpen_kernel =            {{ 0,   -1,    0}, {-1,    5,   -1}, { 0,   -1,    0}};
const std::vector< std::vector<int>> edge_kernel_weak =          {{ 1,    0,   -1}, { 0,    0,    0}, {-1,    0,    0}};
const std::vector< std::vector<int>> edge_kernel_normal =        {{ 0,    1,    0}, { 1,   -4,    1}, { 0,    1,    0}};
const std::vector< std::vector<int>> edge_kernel_strong =        {{-1,   -1,   -1}, {-1,    8,   -1}, {-1,   -1,   -1}};
const std::vector< std::vector<int>> gaussian_blur =             {{ 1,    2,    1}, { 2,    4,    2}, { 1,    2,    1}};
const std::vector< std::vector<int>> roberts_h_kernel =          {{ 1,    0}, { 0,   -1}};
const std::vector< std::vector<int>> roberts_v_kernel =          {{ 0,    1}, {-1,    0}};

const std::vector< std::vector<int>> prewitt_h_kernel =            {{ -1,    0,   1}, { -1,    0,   1}, { -1,    0,   1}};
const std::vector< std::vector<int>> prewitt_v_kernel =            {{ -1,    -1,    -1}, { 0,    0,    0}, {1,   1,   1}};

std::unique_ptr< cv::Mat > ImageConvolute(cv::Mat& src, const std::vector< std::vector< int > >& kernel, const double multiplier = 1.0);
std::unique_ptr< cv::Mat > ApplyEdgeDetection(cv::Mat& src, const std::vector<std::vector<int>>& hKernel, const std::vector<std::vector<int>>& vKernel);
std::unique_ptr<cv::Mat> ApplyRobertsEdgeDetection(cv::Mat& src);

std::unique_ptr<cv::Mat> ThresholdImage(cv::Mat& src, int threshold);

std::unique_ptr<cv::Mat> FindZeroCrossings(cv::Mat& src);

std::vector<std::vector<int>> CalculateLaplacianOfGaussianKernel(int size, double sigma = 1.4);
std::vector<std::vector<int>> CalculateGaussianKernel(int size, double sigma = 1.0);

#endif //IMAGE_PROCESSING_H