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
    Image cross_without_phase = fourier1.inverseWithoutPhase();
    imwrite(cross_without_phase, "../out/ca1/5/cross_without_phase.pgm");

    Image lenna_img = imread("../src/ca1/Lenna.pgm");
    transform::Fourier fourier2(lenna_img);
    Image lenna_without_phase = fourier2.inverseWithoutPhase();
    imwrite(lenna_without_phase, "../out/ca1/5/lenna_without_phase.pgm");

    return 0;
}
