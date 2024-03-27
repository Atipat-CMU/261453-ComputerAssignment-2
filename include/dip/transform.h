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
            vector<vector<complex<double>>> freq_domain;

            double log_normalize(double);
        public:
            Fourier();
            ~Fourier();

            void transform(Image);
            Image getSpectrumImg();
            Image getPhaseImg();
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

        vector<vector<double>> input(numRows, vector<double>(numCols));
        
        for(int row = 0; row < this->numRows; row++){
            for(int col = 0; col < this->numCols; col++){
                input[row][col] = (double)image.get(row, col) * pow(-1, row+col);
            }
        }

        this->freq_domain = dip::signal::fft2D(input);
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
}}

#endif
