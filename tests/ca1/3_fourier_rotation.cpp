#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");
    Image cross_rt_img = rotate(cross_img, 30);
    imwrite(cross_rt_img, "../out/ca1/3/Cross_rotate30.pgm");

    transform::Fourier fourier(cross_rt_img);

    Image cross_rt_amplitude_img = fourier.getSpectrumImg();
    imwrite(cross_rt_amplitude_img, "../out/ca1/3/cross_rt30_amplitude.pgm");

    Image cross_rt_phase_img = fourier.getPhaseImg();
    imwrite(cross_rt_phase_img, "../out/ca1/3/cross_rt30_phase.pgm");

    return 0;
}
