#include "ppm.h"
#include "pgm.h"
#include <iostream>
#include <fstream>

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames)
{   
    std::ifstream is(filename);
    if(!is)
        return false;
    // STEAM HEADER
    int W, H;
    std::string A, X, F, I, C("420mpeg2");
    std::string magicNumber, line;
    is >> magicNumber;
    if(magicNumber != "YUV4MPEG2")
        return false;
    char c;
    for(int i=0;i<7;i++){
        c = is.get();
        
        
        switch (c)
        {
        case 'A':
            is >> A;
            break;
        case 'I':
            is >> I;
            break;
        case 'X':
            is >> X;
            break;
        case 'F':
            is >> F;
            break;
        case 'H':
            is >> H;
            break;
        case 'W':
            is >> W;
            break;
        case 'C':
            is >> C;
            break;

        default:
            break;
        }

        
    }
    mat<uint8_t> frame(H, W);
    std::cout << H << ',' << W << std::endl;
    

   while(true){
       is >> line;
       
       if(is.eof())
            break;
        std::string frameI, frameX;
        for(int i=0;i<2;i++){
            c = is.get();
            switch (c)
            {
            case 'I':
                is >> frameI;
                break;
            case 'X':
                is >> frameX;
                break;
            
            default:
                break;
            }
        }
        // in greyscale images I have only the Y channels, hence we need to fill only 1 matrix.

        is.read(frame.rawdata(), frame.rawsize());
        frames.push_back(frame);
        
   }
   return true;
}

// int main(){
//     std::string filename("utils/test1.y4m");
//     std::vector<mat<uint8_t> > frames;
//     bool val = y4m_extract_gray(filename, frames);
//     std::cout << frames.size();
//     return val;
// }