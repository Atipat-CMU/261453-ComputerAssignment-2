#ifndef TRANFORM_H
#define TRANFORM_H

#include <complex>

#include "../object/Image.h"
#include "../algorithm/FFT.h"

namespace dip { namespace transform {
    class Fourier
    {
        private:
            int numRows;
            int numCols;
            int N;
            vector<vector<complex<double>>> freq_domain;

            double log_normalize(double);
        public:
            Fourier();
            ~Fourier();

            void transform(Image);
            Image inverse();
            Image getSpectrumImg();
            Image getPhaseImg();

            void shift(int x, int y);
    };

    Fourier::Fourier()
    {
    }
    
    Fourier::~Fourier()
    {
    }

    double Fourier::log_normalize(double value){
        if(value <= 0) return value;
        return log10(value);
    }

    void Fourier::transform(Image image){
        this->numRows = image.rows();
        this->numCols = image.cols();

        int power = 0;
        if(numRows < numCols) power = ceil(log2(numCols));
        else power = ceil(log2(numRows));

        this->N = pow(2, power);

        vector<vector<double>> input(N, vector<double>(N));
        
        for(int row = 0; row < N; row++){
            for(int col = 0; col < N; col++){
                if(row >= numRows || col >= numCols){
                    input[row][col] = (double)0 * pow(-1, row+col);
                }else input[row][col] = (double)image.get(row, col) * pow(-1, row+col);
            }
        }

        this->freq_domain = dip::signal::fft2D(input);
    }

    Image Fourier::inverse(){
        vector<vector<double>> output = dip::signal::ifft2D(freq_domain);

        Image out_image(numRows, numCols);

        for(int row = 0; row < this->numRows; row++){
            for(int col = 0; col < this->numCols; col++){
                out_image.set(row, col, (int)(output[row][col] / pow(-1, row+col)));
            }
        }

        return out_image;
    }

    Image Fourier::getSpectrumImg(){
        Image spectrum_img(numRows, numCols);

        double maxVal = log_normalize(abs(freq_domain[0][0]));
        double minVal = log_normalize(abs(freq_domain[0][0]));
        for (const auto& row : freq_domain) {
            for (complex<double> freq : row) {
                double amplitude = log_normalize(abs(freq));
                if (amplitude > maxVal) {
                    maxVal = amplitude;
                }
                if (amplitude < minVal) {
                    minVal = amplitude;
                }
            }
        }

        for(int row = 0; row < this->numRows; row++){
            for(int col = 0; col < this->numCols; col++){
                int spectrum = static_cast<int>(255 * (log_normalize(abs(freq_domain[row][col])) - minVal) / (maxVal-minVal));
                spectrum_img.set(row, col, spectrum);
            }
        }

        return spectrum_img;
    }

    Image Fourier::getPhaseImg(){
        Image phase_img(numRows, numCols);

        double maxVal = log_normalize(arg(freq_domain[0][0]));
        double minVal = log_normalize(arg(freq_domain[0][0]));
        for (const auto& row : freq_domain) {
            for (complex<double> freq : row) {
                double phase = log_normalize(arg(freq));
                if (phase > maxVal) {
                    maxVal = phase;
                }
                if (phase < minVal) {
                    minVal = phase;
                }
            }
        }

        for(int row = 0; row < this->numRows; row++){
            for(int col = 0; col < this->numCols; col++){
                int spectrum = static_cast<int>(255 * (log_normalize(arg(freq_domain[row][col])) - minVal) / (maxVal-minVal));
                phase_img.set(row, col, spectrum);
            }
        }

        return phase_img;
    }

    void Fourier::shift(int x, int y){
        for(int row = 0; row < N; row++){
            for(int col = 0; col < N; col++){
                double j = -2 * M_PI * ((x * row + y * col) / double(N));
                complex<double> phase_shift = polar(1.0, j);
                this->freq_domain[row][col] *= phase_shift;
            }
        }
    }
}}

#endif
