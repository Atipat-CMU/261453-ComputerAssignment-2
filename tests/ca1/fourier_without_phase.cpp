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
    Image cross_without_phase = fourier.inverseWithoutPhase();
    imwrite(cross_without_phase, "../out/ca1/cross_without_phase.pgm");

    Image lenna_img = imread("../src/ca1/Lenna.pgm");
    fourier.transform(lenna_img);
    Image lenna_without_phase = fourier.inverseWithoutPhase();
    imwrite(lenna_without_phase, "../out/ca1/lenna_without_phase.pgm");

    return 0;
}
