#include "utils/pgm.h"
#include "utils/ppm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string.h>

bool y4m_extract_grey(const std::string& filename, std::vector<mat<uint8_t> >& frames){
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cerr << "Error opening the file: " << filename << std::endl;
        return false;
    }
    std::string header;
    is >> header;
    if(strcmp(&header[0], "YUV4MPEG2") != 0)
        return false;
    int w, h;
    std::string C, I, X,f, a;
    for(size_t k=0;k<7;k++){
        int tag = is.get();
        switch (tag)
        {
        case 'W':
            is >> w;
            break;
        case 'H':
            is >> h;
            break;
        case 'C':
            is >> C;
            break;
        case 'I':
            is >> I;
            break;
        case 'F':
            is >> f;
            break;
        case 'A':
            is >> a;
            break;
        case 'X':
            is >> X;
            break;
        
        default:
            break;
        }
    }
    std::cout << "W: " << w << " " << " H: " << h << std::endl;
    std::cout << "A: " << a << " " << " F: " << f << std::endl;
    std::cout << "I: " << I << " " << " X: " << X << std::endl;
    std::cout << "C: " << C << " " << std::endl;
    while(true){
        if(is.eof())
            break;
        std::string fr;
        is >> fr;
        std::cout << fr << std::endl;
        std::string frame_i, frame_x;
        for(size_t j=0;j<2;j++){
            int frame_tag = is.get();
            switch (frame_tag)
            {
            break;case 'I':
                is >> frame_i;
                break;
            case 'X':
                is >> frame_x;
                break;
            
            default:
                break;
            }
            std::cout << frame_tag << " frame headers: " << frame_x << frame_i << std::endl;
        }
        mat<uint8_t> frame(w, h);
        std::string data;
        is >> data;
        
        //frames.push_back(frame);

    }
    
    return false;

   
}

int main(){
    std::vector<mat<uint8_t> > frames;
    if(y4m_extract_grey("foreman_cif.y4m", frames)){
        for(int i=0;i<frames.size();i++){
            std::stringstream ss;
            ss <<  std::setfill('0');
            ss << "frame" << std::setw(3) << i << ".pgm";
            save_pgm(ss.str(), frames[i]);
        }
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}