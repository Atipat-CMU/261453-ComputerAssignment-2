#include "../../include/dip.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    transform::Fourier fourier;

    Image cross_img = imread("../src/ca1/Cross.pgm");
    fourier.transform(cross_img);
    Image cross_without_amplitude = fourier.inverseWithoutAmplitude();
    imwrite(cross_without_amplitude, "../out/ca1/cross_without_amplitude.pgm");

    Image lenna_img = imread("../src/ca1/Lenna.pgm");
    fourier.transform(lenna_img);
    Image lenna_without_amplitude = fourier.inverseWithoutAmplitude();
    imwrite(lenna_without_amplitude, "../out/ca1/lenna_without_amplitude.pgm");

    return 0;
}
