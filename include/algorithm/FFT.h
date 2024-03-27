#ifndef FFT_H
#define FFT_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

namespace signal {
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
}

#endif
