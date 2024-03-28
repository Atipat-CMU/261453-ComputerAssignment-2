#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");
    Image cross_rt_img = downSample(cross_img, 2);
    imwrite(cross_rt_img, "../out/ca1/4/Cross_downsample2.pgm");

    transform::Fourier fourier(cross_rt_img);

    Image cross_rt_amplitude_img = fourier.getSpectrumImg();
    imwrite(cross_rt_amplitude_img, "../out/ca1/4/cross_ds2_amplitude.pgm");

    Image cross_rt_phase_img = fourier.getPhaseImg();
    imwrite(cross_rt_phase_img, "../out/ca1/4/cross_ds2_phase.pgm");

    return 0;
}
