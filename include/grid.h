#ifndef GRID_H
#define GRID_H

#include <string>
#include <stack>
#include <queue>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./object/Image.h"
#include "./object/Mask.h"

using namespace std;

namespace dip {
    Mask gridIntersect(Image image){
        Mask mask = Mask(image.rows(), image.cols());

        for(int row = 0; row < image.rows(); row++){
            for(int col = 0; col < image.cols(); col++){
                int top = image.get(row-1,col);
                int left = image.get(row,col-1);
                int right = image.get(row,col+1);
                int bottom = image.get(row+1,col);
                
                if(top <= 32 && left <= 32 && right <= 32 && bottom <= 32){
                    mask.set(row, col, 1);
                }
                if((top == -1 && bottom <= 32) ||(top <= 32 && bottom == -1)){
                    mask.set(row, col, 1);
                }
                if((left == -1 && right <= 32) ||(left <= 32 && right == -1)){
                    mask.set(row, col, 1);
                }
            }
        }
        return mask;
    }

    vector<Pixel> gridFormTextFile(string filename){
        vector<Pixel> grid_points;

        string line;
        ifstream myfile(filename);

        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                istringstream iss(line);
                string x_str, y_str;

                if (getline(iss, x_str, ' ') && getline(iss, y_str, ' ')) {
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    grid_points.emplace_back(Pixel(x, y));
                }
            }
            myfile.close();
        }
        return grid_points;
    }

    int gridGetNeighbor(Mask& inner_mask, int row, int col) {
        struct Point {
            int row;
            int col;
        };

        std::queue<Point> queue;
        queue.push({row, col});

        while (!queue.empty()) {
            Point current = queue.front();
            queue.pop();

            if (inner_mask.get(current.row - 1, current.col) > 0) {
                return inner_mask.get(current.row - 1, current.col);
            }else if (current.row > 0) queue.push({current.row - 1, current.col});
            if (inner_mask.get(current.row + 1, current.col) > 0) {
                return inner_mask.get(current.row + 1, current.col);
            }else if (current.row < inner_mask.rows() - 1) queue.push({current.row + 1, current.col});
            if (inner_mask.get(current.row, current.col - 1) > 0){
                return inner_mask.get(current.row, current.col - 1);
            }else if (current.col > 0) queue.push({current.row, current.col - 1});
            if (inner_mask.get(current.row, current.col + 1) > 0) {
                return inner_mask.get(current.row, current.col + 1);
            }else if (current.col < inner_mask.cols() - 1) queue.push({current.row, current.col + 1});
        }

        return -1;
    }

    void gridColoringDFS(const Image& image, Mask& mask, int row, int col, int value) {
        struct Point {
            int row;
            int col;
        };

        std::stack<Point> stack;
        stack.push({row, col});

        while (!stack.empty()) {
            Point current = stack.top();
            stack.pop();

            mask.set(current.row, current.col, value);

            if (current.row > 0 && mask.get(current.row - 1, current.col) == 0 && image.get(current.row - 1, current.col) == 255) {
                stack.push({current.row - 1, current.col});
            }if (current.row < image.rows() - 1 && mask.get(current.row + 1, current.col) == 0 && image.get(current.row + 1, current.col) == 255) {
                stack.push({current.row + 1, current.col});
            }if (current.col > 0 && mask.get(current.row, current.col - 1) == 0 && image.get(current.row, current.col - 1) == 255) {
                stack.push({current.row, current.col - 1});
            }if (current.col < image.cols() - 1 && mask.get(current.row, current.col + 1) == 0 && image.get(current.row, current.col + 1) == 255) {
                stack.push({current.row, current.col + 1});
            }
        }
    }

    Pixel intersection(Pixel l11, Pixel l12, Pixel l21, Pixel l22){
        if((l12.x() - l11.x()) != 0 && (l22.x() - l21.x()) != 0){
            double m1 = (double)(l12.y() - l11.y()) / (l12.x() - l11.x());
            double c1 = l11.y() - (l11.x() * m1);
            double m2 = (double)(l22.y() - l21.y()) / (l22.x() - l21.x());
            double c2 = l21.y() - (l21.x() * m2);

            double x = (c2 - c1) / (m1 - m2);
            double y = m1 * x + c1;

            return Pixel((int)x, (int)y);
        }else{
            double m1 = (double)(l12.x() - l11.x()) / (l12.y() - l11.y());
            double c1 = l11.x() - (l11.y() * m1);
            double m2 = (double)(l22.x() - l21.x()) / (l22.y() - l21.y());
            double c2 = l21.x() - (l21.y() * m2);

            double y = (c2 - c1) / (m1 - m2);
            double x = m1 * y + c1;

            return Pixel((int)x, (int)y);
        }
    }

    Mask gridBoxing(const Image& image, vector<Pixel> grid_points) {
        Mask mask(image.rows(), image.cols());
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 16; j++) {
                Pixel tl = grid_points[j + (i * (16+1))];
                Pixel tr = grid_points[(j + 1) + (i * (16+1))];
                Pixel bl = grid_points[j + ((i + 1) * (16+1))];
                Pixel br = grid_points[(j + 1) + ((i + 1) * (16+1))];

                Pixel center = intersection(tl, br, tr, bl);
                gridColoringDFS(image, mask, center.x(), center.y(), (j + 1) + (i * 16));
            }
        }

        Mask boder_mask(image.rows(), image.cols());
        for(int row = 0; row < mask.rows(); row++){
            for(int col = 0; col < mask.cols(); col++){
                if(mask.get(row, col) == 0){
                    boder_mask.set(row, col, gridGetNeighbor(mask, row, col));
                }
            }
        }

        for(int row = 0; row < mask.rows(); row++){
            for(int col = 0; col < mask.cols(); col++){
                if(mask.get(row, col) == 0){
                    mask.set(row, col, boder_mask.get(row, col));
                }
            }
        }

        return mask;
    }


}

#endif
