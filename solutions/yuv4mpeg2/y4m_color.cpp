#include "ppm.h"
#include "pgm.h"
#include <fstream>
#include <iostream>

void YCbCr_to_RGB(mat<uint8_t>& Y, mat<uint8_t>& Cb, mat<uint8_t>& Cr,mat<vec3b>& frame)
{
    for(int i=0;i<Y.rows();i++){
        for(int j=0;j<Y.cols();j++){
            // Y in [16, 235], Cb,Cr in [16, 240]
            int y = Y(i, j) < 16?16:Y(i, j)>235?235:Y(i, j);
            int cb = Cb(i, j) < 16?16:Cb(i, j)>240?240:Cb(i, j);
            int cr = Cr(i, j) < 16?16:Cr(i, j)>240?240:Cr(i, j);

            y -= 16;
            cb -= 128;
            cr -= 128;

            // convert to RGB

            int r, g, b;
            r = 1.164*y + 1.596*cr;
            g = 1.164*y - 0.392*cb - 0.813*cr;
            b = 1.164*y + 2.017*cb;

            // flatten r,g,b between 0 and 255
            r = r > 255?255:r<0?0:r;
            g = g > 255?255:g<0?0:g;
            b = b > 255?255:b<0?0:b;

            frame(i, j)[0] = r;
            frame(i, j)[1] = g;
            frame(i, j)[2] = b;

        }
    }
    
}

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames)
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
    mat<uint8_t> Y(H, W);
    mat<uint8_t> Cb(H/2, W/2);
    mat<uint8_t> Cr(H/2, W/2);

    mat<vec3b> frame(H, W);

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

        is.read(Y.rawdata(), Y.rawsize());
        is.read(Cb.rawdata(), Cb.rawsize());
        is.read(Cr.rawdata(), Cr.rawsize());

        YCbCr_to_RGB(Y, Cb, Cr, frame);
        frames.push_back(frame);
        
   }
   return true;
}

// int main(){
//     std::string filename("foreman_cif.y4m");
//     std::vector<mat<vec3b> > frames;
//     bool val = y4m_extract_color(filename, frames);
//     std::cout << frames.size();
//     return val;
// }