#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image chess_img = imread("../src/ca1/Chess.pgm");

    transform::Fourier fourier;
    fourier.transform(chess_img);

    Kernel kernel({
        {1/9.0,1/9.0,1/9.0},
        {1/9.0,1/9.0,1/9.0},
        {1/9.0,1/9.0,1/9.0}
    });

    // Filter filter({
    //     {0, -1, 0},
    //     {-1, 4,-1},
    //     {0, -1, 0}
    // });

    Image filter_amplitude_img = kernel.getSpectrumImg(256);
    imwrite(filter_amplitude_img, "../out/ca1/filter_amplitude.pgm");

    Image filter_phase_img = kernel.getPhaseImg(256);
    imwrite(filter_phase_img, "../out/ca1/filter_phase.pgm");

    fourier.applyKernel(kernel);
    Image chess_shift_img = fourier.inverse();
    imwrite(chess_shift_img, "../out/ca1/Chess_filter_fourier_apply.pgm");

    return 0;
}
