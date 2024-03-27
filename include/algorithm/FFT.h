#ifndef FFT_H
#define FFT_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

namespace dip { namespace signal {
    // Function to perform 1D FFT along the rows of a 2D matrix
    void fft1D(vector<complex<double>>& row) {
        int N = row.size();
        if (N <= 1) {
            return;
        }

        // Split even and odd elements
        vector<complex<double>> even(N/2), odd(N/2);
        for (int i = 0; i < N/2; ++i) {
            even[i] = row[i * 2];
            odd[i] = row[i * 2 + 1];
        }

        // Recursively compute FFT on even and odd halves
        fft1D(even);
        fft1D(odd);

        // Combine results of even and odd halves
        for (int k = 0; k < N/2; ++k) {
            complex<double> t = polar(1.0, -2.0 * M_PI * k / N) * odd[k];
            row[k] = even[k] + t;
            row[k + N/2] = even[k] - t;
        }
    }

    // Function to perform 2D FFT
    vector<vector<complex<double>>> fft2D(const vector<vector<double>>& input) {
        int numRows = input.size();
        int numCols = input[0].size();

        vector<vector<complex<double>>> output(numRows, vector<complex<double>>(numCols));

        // FFT along the rows
        for (int i = 0; i < numRows; ++i) {
            vector<complex<double>> row(numCols);
            for (int j = 0; j < numCols; ++j) {
                row[j] = complex<double>(input[i][j], 0);
            }
            fft1D(row);
            output[i] = row;
        }

        // Transpose the result
        vector<vector<complex<double>>> transposed(numCols, vector<complex<double>>(numRows));
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                transposed[j][i] = output[i][j];
            }
        }

        // FFT along the columns
        for (int i = 0; i < numCols; ++i) {
            fft1D(transposed[i]);
        }

        // Transpose back the result
        for (int i = 0; i < numCols; ++i) {
            for (int j = 0; j < numRows; ++j) {
                output[j][i] = transposed[i][j];
            }
        }

        return output;
    }

    // Function to perform 1D inverse FFT
    void ifft1D(vector<complex<double>>& spectrum) {
        int N = spectrum.size();
        if (N <= 1) {
            return;
        }

        // Conjugate the spectrum
        for (auto& val : spectrum) {
            val = conj(val);
        }

        // Perform forward FFT
        fft1D(spectrum);

        // Scale the result
        for (auto& val : spectrum) {
            val /= N;
        }
    }

    // Function to perform 2D inverse FFT
    vector<vector<double>> ifft2D(const vector<vector<complex<double>>>& spectrum) {
        int numRows = spectrum.size();
        int numCols = spectrum[0].size();

        vector<vector<complex<double>>> transposed(numCols, vector<complex<double>>(numRows));

        // Transpose the spectrum
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                transposed[j][i] = spectrum[i][j];
            }
        }

        // IFFT along the columns
        for (int i = 0; i < numCols; ++i) {
            ifft1D(transposed[i]);
        }

        // Transpose back the result
        vector<vector<complex<double>>> transposedBack(numRows, vector<complex<double>>(numCols));
        for (int i = 0; i < numCols; ++i) {
            for (int j = 0; j < numRows; ++j) {
                transposedBack[j][i] = transposed[i][j];
            }
        }

        // IFFT along the rows
        vector<vector<double>> output(numRows, vector<double>(numCols));
        for (int i = 0; i < numRows; ++i) {
            vector<complex<double>>& row = transposedBack[i];
            ifft1D(row);
            for (int j = 0; j < numCols; ++j) {
                output[i][j] = row[j].real(); // Take real part
            }
        }

        return output;
    }
}}

#endif
