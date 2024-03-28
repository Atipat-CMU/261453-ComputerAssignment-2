#include "../../include/dip.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");

    transform::Fourier fourier(cross_img);

    Image cross_amplitude_img = fourier.getSpectrumImg();
    imwrite(cross_amplitude_img, "../out/ca1/1/cross_ori_amplitude.pgm");

    Image cross_phase_img = fourier.getPhaseImg();
    imwrite(cross_phase_img, "../out/ca1/1/cross_ori_phase.pgm");

    return 0;
}
