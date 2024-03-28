#include "../../include/dip.h"

#include <iostream>

using namespace std;
using namespace dip;
using namespace transform;

int main(int argc, char const *argv[])
{
    Image chess_noise_img = imread("../src/ca2/Chess_noise.pgm");

    Image chess_median_img = medianFilter(chess_noise_img, 3);
    imwrite(chess_median_img, "../out/ca2/2/Chess_median_3.pgm"); 

    return 0;
}
