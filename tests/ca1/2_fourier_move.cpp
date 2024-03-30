#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");

    transform::Fourier fourier(cross_img);

    fourier.shift(20, 30);
    Image cross_shift_img = fourier.inverse();
    imwrite(cross_shift_img, "../out/ca1/2/Cross_shift.pgm");

    Image cross_amplitude_img = fourier.getSpectrumImg();
    imwrite(cross_amplitude_img, "../out/ca1/2/cross_shift_amplitude.pgm");

    Image cross_phase_img = fourier.getPhaseImg();
    imwrite(cross_phase_img, "../out/ca1/2/cross_shift_phase.pgm");

    return 0;
}
