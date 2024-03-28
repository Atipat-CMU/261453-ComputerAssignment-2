#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image chess_img = imread("../src/ca1/Chess.pgm");

    transform::Fourier fourier(chess_img);

    Kernel kernel({
        {1/9.0,1/9.0,1/9.0},
        {1/9.0,1/9.0,1/9.0},
        {1/9.0,1/9.0,1/9.0}
    });

    // Kernel kernel({
    //     {0, -1, 0},
    //     {-1, 4,-1},
    //     {0, -1, 0}
    // });

    Image kernel_amplitude_img = kernel.getSpectrumImg(256);
    imwrite(kernel_amplitude_img, "../out/ca1/7/kernel_amplitude.pgm");

    Image kernel_phase_img = kernel.getPhaseImg(256);
    imwrite(kernel_phase_img, "../out/ca1/7/kernel_phase.pgm");

    fourier.applyKernel(kernel);
    Image chess_fourier_img = fourier.inverse();
    imwrite(chess_fourier_img, "../out/ca1/7/Chess_kernel_fourier_apply.pgm");

    Image chess_conv_img = convolution(chess_img, kernel);
    imwrite(chess_conv_img, "../out/ca1/7/Chess_kernel_convolution.pgm"); 

    return 0;
}
