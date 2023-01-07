#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <iostream>
#include <cmath>
#include <complex>
#include "Palette.h"
#include <opencv2/opencv.hpp>

int lerp(int a, int b, double t);
RGBColor interpolate_color(RGBColor c1, RGBColor c2, double t);
void zoom(int i);
void point_test(int x, int y, int i);
void print_mat(cv::Mat mat);
RGBColor mandelbrot(std::complex<double> c);
void plot(cv::Mat &image, int x, int y, RGBColor c);


#endif