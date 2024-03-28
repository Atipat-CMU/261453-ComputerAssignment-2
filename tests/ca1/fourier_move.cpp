#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image cross_img = imread("../src/ca1/Cross.pgm");

    transform::Fourier fourier;
    fourier.transform(cross_img);

    fourier.shift(20, 30);
    Image cross_shift_img = fourier.inverse();
    imwrite(cross_shift_img, "../out/ca1/Cross_shift.pgm");

    return 0;
}
