#include "../../include/dip.h"

#include <iostream>
#include <cmath>
#include <complex>

using namespace std;
using namespace dip;
using namespace transform;

complex<double> ideal_lowpass_filter(int u, int v, int M, int N){
    double D0 = 50;
    double D = sqrt(pow(u-(M/2.0),2)+pow(v-(N/2.0),2));
    if (D <= D0) return 1;
    return 0;
}

complex<double> butterworth_lowpass_filter(int u, int v, int M, int N){
    double D0 = 20;
    int n = 2;
    double D = sqrt(pow(u-(M/2.0),2)+pow(v-(N/2.0),2));
    return 1/(1+pow(D/D0,2*n));
}

complex<double> gaussian_lowpass_filter(int u, int v, int M, int N){
    double D0 = 20;
    double D = sqrt(pow(u-(M/2.0),2)+pow(v-(N/2.0),2));
    return exp(-(pow(D,2)/(2*pow(D0,2))));
}

int main(int argc, char const *argv[])
{
    Image chess_img = imread("../src/ca2/Cross.pgm");

    transform::Fourier fourier(chess_img);

    fourier.applyFilter(gaussian_lowpass_filter);

    Image chess_shift_img = fourier.inverse();
    imwrite(chess_shift_img, "../out/ca2/1/cross_glf_20.pgm");

    return 0;
}
