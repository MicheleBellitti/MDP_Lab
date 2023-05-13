#include "pgm.h"
#include "ppm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string.h>

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b> >& frames){
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
    std::string C("420jpeg"), I, X,f, a;
    while(true){
        char next = is.get();
		if (next == '\n') {
			break;
		}
		if (next != ' ') {
			return false;
		}
		char tag = is.get();
        switch (tag) {
		break; case 'W': is >> w;
		break; case 'H': is >> h;
		break; case 'C': is >> C;
		break; case 'I': is >> I;
		break; case 'F': is >> f;
		break; case 'A': is >> a;
		break; case 'X': is >> X;
		break; default: return false;
		}
    }
    std::cout << "W: " << w << " " << " H: " << h << std::endl;
    std::cout << "A: " << a << " " << " F: " << f << std::endl;
    std::cout << "I: " << I << " " << " X: " << X << std::endl;
    std::cout << "C: " << C << std::endl;
    
    mat<uint8_t> Y(h, w);
    mat<uint8_t> Cb(h/2, w/2);
    mat<uint8_t> Cr(h/2, w/2);
    mat<vec3b> frame_RGB(h, w);
    std::cout << "Reading frames..." << std::endl;
    while(is >> header){

        if(header != "FRAME")
            break;

        std::string frame_i, frame_x;
        while (true) {
			char next = is.get();
			if (next == '\n') {
				break;
			}
			if (next != ' ') {
                std::cerr << "Error reading header of frame #" << frames.size() << std::endl;
				return false;
			}
			char tag = is.get();
			switch (tag) {
			break; case 'I': is >> frame_i;
			break; case 'X': is >> frame_x;
			break; default: return false;
			}
		}

        is.read(Y.rawdata(), Y.rawsize());
        is.read(Cb.rawdata(), Cb.rawsize());
        is.read(Cr.rawdata(), Cr.rawsize());



        // convert from YCbCr to RGB
        for(int i=0;i<Y.rows();i++){

            for(int j=0;j<Y.cols();j++){

                int y = Y(i, j)<16?16:Y(i,j)>235?235:Y(i, j);

                
                int cb = Cb(i/2, j/2);
                cb = cb<16?16:cb>240?240:cb;


                int cr = Cr(i/2, j/2);
                cr = cr<16?16:cr>240?240:cr;

                int r = 1.164 * (y - 16) + 1.596 * (cr - 128);
                int g = 1.164 * (y - 16) - 0.813 * (cr - 128) - 0.392 * (cb - 128);
                int b = 1.164 * (y - 16) + 2.017 * (cb - 128);
                
                r = r < 0 ? 0:r > 255 ? 255:r;
                g = g < 0 ? 0:g > 255 ? 255:g;
                b = b < 0 ? 0:b > 255 ? 255:b;

                frame_RGB(i, j) = {(uint8_t)r, (uint8_t)g, (uint8_t)b};
            }
        }

        
        frames.push_back(frame_RGB);
        if(is.eof())
            break;

    }

    std::cout << "frames: " << frames.size() << std::endl;
    
    return true;

   
}



/*
int main(){
    std::vector<mat<vec3b> > frames;
    std::string filename = "foreman_cif.y4m";
    if(y4m_extract_color(filename, frames)){
        for(size_t i=0;i<frames.size();i++){
            std::stringstream ss;
            ss << "frames/" << filename << "_" << "frame" << i << ".ppm";
            save_ppm(ss.str(), frames[i]);
        }
        std::cerr << "...True has been returned..\n";
        return EXIT_SUCCESS;
    }
    std::cerr << "Something went wrong...False has been returned..\n";
    return EXIT_FAILURE;
}*/