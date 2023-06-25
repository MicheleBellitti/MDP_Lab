#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

struct BinaryImage{
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    BinaryImage(): W(0), H(0) {}
    BinaryImage(int w, int h): W(w), H(h) { ImageData = std::vector<uint8_t>(W*H); }

    uint8_t& operator[] (int idx){
        return ImageData[idx];
    }

    bool ReadFromPBM(const std::string& filename);
};

struct Image{
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    Image(): W(0), H(0) {}
    Image(int w, int h): W(w), H(h) { ImageData = std::vector<uint8_t>(W*H); }

    uint8_t& operator[] (int idx){
        return ImageData[idx];
    }

    
};

class BitReader{
    std::istream& is_;
    uint8_t buf_, n_=0;

    public:

        BitReader(std::istream& is): is_(is) {}

        uint8_t readbit(){
            if(n_ == 0){
                is_.read(reinterpret_cast<char*>(&buf_), 1);
                n_ = 8;
            }
            n_ --;
            return (buf_ >> 1) &  1;
        }
        uint32_t read(uint32_t n){
            uint32_t r = 0;
            while(n -->0){
                r = r | readbit();
            }
            return r;
        }

        bool fail(){
            return is_.fail();
        }
};

extern Image BinaryImageToImage(const BinaryImage& bimg);
 