#include "../../include/dip.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");
    transform::Fourier fourier1(cross_img);
    Image cross_without_amplitude = fourier1.inverseWithoutAmplitude();
    imwrite(cross_without_amplitude, "../out/ca1/5/cross_without_amplitude.pgm");

    Image lenna_img = imread("../src/ca1/Lenna.pgm");
    transform::Fourier fourier2(lenna_img);
    Image lenna_without_amplitude = fourier2.inverseWithoutAmplitude();
    imwrite(lenna_without_amplitude, "../out/ca1/5/lenna_without_amplitude.pgm");

    return 0;
}
