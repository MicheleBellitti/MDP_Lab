#include "matrix.h"
T T::operator+ (T& other){
    T res;
    res.r = r + other.r;
    res.g = g + other.g;
    res.b = b + other.b;
    return res;
}

T T::operator* (T& other){
    T res;
    res.r = r * other.r;
    res.g = g * other.g;
    res.b = b * other.b;
    return res;
}

Matrix::Matrix(int rows, int cols){
    if(rows == 0 || cols == 0){
        throw std::out_of_range("Index out of range");
    }
    rows_ = rows;
    cols_ = cols;
    data_ = std::vector<std::vector<T> >(rows_, std::vector<T>(cols));
}
size_t Matrix::rows(){
    return rows_;
}

size_t Matrix::cols(){
    return cols_;
}

size_t Matrix::size(){
    return rows_*cols_;
}


std::vector<T> Matrix::getDiagonal(bool principal){
    // check whether the matrix is squared or not
    if(rows_ != cols_){
        std::cout << "Error: Trying to get the diagonal of a non-squared matrix(not possible!)" << std::endl;
        return std::vector<T>();
    }

    std::vector<T> diag;
    if(principal){
        for(size_t r=0;r<rows_;r++){
            diag.push_back(data_[r][r]);
        }
    }
    else{
        for(size_t r=0;r<rows_;r++){
            diag.push_back(data_[r][ cols_ - 1 - r]);
        }
    }
    return diag;
}
T Matrix::det(){
    // check if the matrix is squared, otherwise print an error and end the function
    if(rows_ != cols_){
        std::cout << "Error: Trying to compute the determinant of a non-squared matrix(not possible!)" << std::endl;
        exit(1);
    }

    T prod = {1, 1, 1};
    for(size_t r=0;r<rows_;r++){
        for(size_t c=0;c<cols_;c++){
            if(r == c){
                prod = prod* data_[r][c];
            }
        }
    }
    return prod;
}

void Matrix::resize(int new_row, int new_col){
    rows_ = new_row;
    cols_ = new_col;

    data_.clear();
    data_.resize(rows_);
    for(auto& row: data_){
        row.resize(cols_);
    }
}
std::vector<T>& Matrix::operator[] (int index){
    if(index < 0 || index >= rows_){
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

T& Matrix::operator() (size_t r_index, size_t c_index){
    if(r_index >= rows_ || c_index >= cols_){
        throw std::out_of_range("Index out of range");
    }
    return data_[r_index][c_index];

}

void Matrix::vflip(){
    for(size_t k=0;k<rows_/2;k++){
        std::swap(data_[k], data_[rows_-k-1]);
    }
}

void Matrix::hflip(){
    for(size_t r=0;r<rows_/2;r++){
        for(size_t c=0;c<cols_/2;c++){
            std::swap(data_[r][c], data_[r][cols_-c-1]);
        }
    }

}

void Matrix::print_mat(){

     // Print the body of the matrix.
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            std::cout << "r: " << data_[i][j].r << " g:"  << data_[i][j].g << " b:" << data_[i][j].b << "||";
        }
    std::cout << std::endl;
    }
}

