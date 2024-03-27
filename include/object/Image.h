#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <iostream>
#include <string>

#include "Vector2D.h"
#include "Pixel.h"
#include "Frame.h"
#include "Mask.h"

using namespace std;

namespace dip {
    class Image
    {
    private:
        Vector2D<int> image;
        int histogram[256];
        int max_val = 0;

        void calHistogram();
        Image operation(Image const&, char);
        Image operation(double, Image const&, char) const;
        bool isOutOfImage(Pixel);
    public:
        Image();
        Image(int, int);
        Image(Vector2D<int>);
        ~Image();

        // function
        int rows() const;
        int cols() const;
        int max() const;
        int area() const;

        Vector2D<char> getRaw();
        Vector2D<int> get();

        void set(int, int, int);
        int get(Pixel);
        int get(int, int);
        int get(int, int) const;
        int getHistFreq(int);
        void printHistogram();

        Image mark(Mask, int);
        Image highlight(Mask);

        Image crop(Frame);
        Image operator+(Image const&);
        Image operator-(Image const&);
        Image operator*(Image const&);
        friend Image operator*(double, Image const&);
        friend Image operator/(Image const&, double);
    };

    
    Image::Image()
    {
    }

    Image::Image(int numrows, int numcols)
    {
        this->image = Vector2D<int>(numrows, numcols);
        this->calHistogram();
    }

    Image::Image(Vector2D<int> image)
    {
        this->image = image;
        this->calHistogram();
    }

    Image::~Image()
    {
    }

    int Image::rows() const{
        return this->image.rows();
    }

    int Image::cols() const{
        return this->image.cols();
    }

    int Image::max() const{
        return this->max_val;
    }

    int Image::area() const{
        return this->rows()*this->cols();
    }

    Vector2D<int> Image::get(){
        return this->image;
    }

    void Image::set(int x, int y, int d){
        this->image.set(x, y, d);
    }

    int Image::get(Pixel p){
        if(isOutOfImage(p)) return -1;
        return this->image.get(p.x(), p.y());
    }

    int Image::get(int x, int y){
        return this->get(Pixel(x,y));
    }

    int Image::get(int x, int y) const{
        return this->image.get(x, y);
    }

    bool Image::isOutOfImage(Pixel p){
        int x = p.x();
        int y = p.y();
        return !(0 <= x && x < this->rows() && 0 <= y && y < this->cols());
    }

    void Image::calHistogram(){
        for(int i = 0; i < 256; i++){
            histogram[i] = 0;
        }
        for(int row = 0; row < this->rows(); ++row) {
            for(int col = 0; col < this->cols(); ++col) {
                int d = this->image.get(row, col);
                this->histogram[d]++;
                if(d > max_val) max_val = d;
            }
        }
    }

    Image Image::mark(Mask mask, int color_value){
        Image new_image(this->image);

        for(int row = 0; row < this->image.rows(); row++){
            for(int col = 0; col < this->image.cols(); col++){
                if(mask.get(row, col) != 0){
                    new_image.set(row, col, color_value);
                }
            }
        }

        return new_image;
    }

    Image Image::highlight(Mask mask){
        Image new_image(this->image);

        for(int row = 0; row < this->image.rows(); row++){
            for(int col = 0; col < this->image.cols(); col++){
                if(mask.get(row, col) != 0){
                    new_image.set(row-1, col, 255);
                    new_image.set(row+1, col, 255);
                    new_image.set(row, col, 0);
                    new_image.set(row, col-1, 255);
                    new_image.set(row, col+1, 255);
                }
            }
        }

        return new_image;
    }

    Image Image::operation(Image const& obj, char op){

        Image new_image(image.rows(), image.cols());

        for(int row = 0; row < image.rows(); row++){
            for(int col = 0; col < image.cols(); col++){
                int img1 = obj.image.get(row, col);
                int img2 = this->get(row, col);
                if(op == '+') new_image.set(row, col, img1+img2);
                else if(op == '-') new_image.set(row, col, img1-img2);
                else if(op == '*') new_image.set(row, col, img1*img2);
                else {new_image.set(row, col, img1); cout << "wrong";}
            }
        }

        return new_image;
    }

    Image Image::operation(double k, Image const& obj, char op) const{

        Image new_image(obj.rows(), obj.cols());

        for(int row = 0; row < obj.rows(); row++){
            for(int col = 0; col < obj.cols(); col++){
                int img1 = obj.image.get(row, col);
                if(op == '*') new_image.set(row, col, img1*k);
                else if(op == '/') new_image.set(row, col, img1/k);
                else {new_image.set(row, col, img1); cout << "wrong";}
            }
        }

        return new_image;
    }

    int Image::getHistFreq(int d){
        return this->histogram[d];
    }

    void Image::printHistogram(){
        for(int i = 0; i < 256; i++){
            cout << i << ": " << this->histogram[i] << endl;
        }
    }

    Image Image::crop(Frame frame){
        Pixel top = frame.getTop();
        Pixel bottom = frame.getBottom();

        int c_rows = bottom.x()-top.x()+1;
        int c_cols = bottom.y()-top.y()+1;

        Vector2D<int> cropedImage = image.crop(top.x(), top.y(), bottom.x(), bottom.y());
        return Image(cropedImage);
    }

    Image Image::operator+(Image const& obj)
    {
        return this->operation(obj, '+');
    }

    Image Image::operator-(Image const& obj)
    {
        return this->operation(obj, '-');
    }

    Image Image::operator*(Image const& obj)
    {
        return this->operation(obj, '*');
    }

    Image operator*(double k, Image const& obj)
    {
        return obj.operation(k, obj, '*');
    }

    Image operator/(Image const& obj, double k)
    {
        return obj.operation(k, obj, '/');
    }
}

#endif
