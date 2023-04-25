#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>


struct T{
    uint8_t r, g, b;
    T(){
        r = 0;
        g = 255;
        b = 0;
    }
    T(uint8_t rr, uint8_t gg, uint8_t bb){
        r = rr;
        g = gg;
        b = gg;
    }
    T operator*(T& other);
    
};
class Matrix {
    
    size_t rows_, cols_;
    std::vector<std::vector<T> > data_;

    public:

        Matrix(int rows, int cols);
        size_t rows();

        size_t cols();

        size_t size();

        std::vector<T> getDiagonal(bool principal=true);
        
        T det();
        
        void resize(int new_row, int new_col);

        std::vector<T>& operator[] (int index);

        T& operator() (size_t r_index, size_t c_index);

        void operator+ (Matrix& other);
        
        void vflip();

        void hflip();

        void print_mat();







};
#endif // MATRIX_H