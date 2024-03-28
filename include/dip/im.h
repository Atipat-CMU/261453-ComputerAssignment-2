#ifndef IM_H
#define IM_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../object/Image.h"

using namespace std;

namespace dip {
    Image imread(string filename){
        int count = 0;

        string version;
        int numrows = 0, numcols = 0;
        int max_val;

        ifstream file(filename, ios::binary);
        stringstream ss;
        stringstream temp;    
        string line;

        if (!file.is_open()) {
            throw runtime_error("File not found: " + filename);
        }

        while (getline(file, line))
        {
            if(line[0] != '#'){
                if(count == 0){ version = line; }
                if(count == 1){ temp << line; temp >> numcols >> numrows; }
                if(count == 2){ temp.clear(); temp << line; temp >> max_val; }
                count++;
            }
            if(count == 3) break;
        }
        
        int imsize = 0;

        Vector2D<int> image = Vector2D<int>(numrows, numcols);

        char pixel;

        for(int row = 0; row < numrows; row++){
            for(int col = 0; col < numcols; col++){
                file.read(&pixel, 1);
                image.set(row, col, static_cast<int>(static_cast<unsigned char>(pixel)));
            }
        }
        file.close();

        return Image(image);
    }

    void imwrite(Image& image, string filename){
        ofstream file(filename, std::ios::binary);
        file << "P5" << endl;
        file << image.cols() << " " << image.rows() << endl;
        file << "255" << endl;

        vector<char> raw1d;

        for(int row = 0; row < image.rows(); row++){
            for(int col = 0; col < image.cols(); col++){
                int d = image.get(row, col);
                if(d < 0) d = 0;
                if(d > 255) d = 255;
                raw1d.push_back(static_cast<unsigned char>(d));
            }
        }

        file.write(raw1d.data(), raw1d.size());
        file.close();
    }

    void imshow(Image& image){
        for(int row = 0; row < image.rows(); ++row) {
            for(int col = 0; col < image.cols(); ++col) {
                cout << image.get(row, col) << " ";
            }
            cout << endl;
        }
    }
}

#endif
