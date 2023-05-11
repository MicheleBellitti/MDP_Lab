#include<iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <array>
#include <cstdint>
#include "mat.h"


bool save_pam(const std::string& filename, const mat<vec3b>& img)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os)
		return false;

	os << "P7" << "\n";
	os << "WIDTH " << img.cols();
    os.put('\n');
    os << "HEIGHT " << img.rows();
    os.put('\n');
    os << "DEPTH 3\n";
    os << "MAXVAL 255\n";
    os << "TUPLTYPE RGB\n";
    os << "ENDHDR\n";

		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				os.put(static_cast<char>(img(r, c)[0]));
                os.put(static_cast<char>(img(r, c)[1]));
                os.put(static_cast<char>(img(r, c)[2]));
			}
		}

	return true;
}

void load_pam(const std::string& filename, mat<uint8_t>& img){
    std::ifstream is(filename,std::ios::binary);
    if(!is){
        std::cerr << "Error opening the pam file: " << filename << std::endl;
        exit(1);
    }
    std::string magicNumber;
    is >> magicNumber;
    if(strcmp(magicNumber.data(),"P7") != 0){
        std::cerr << "Wrong PAM format(magic number incorrect)" << std::endl;
        exit(1);
    }
    size_t width, height;
    std::string line;

    is >> line;
    is >> width;

    is >> line;
    is >> height;


    int channels, maxval;
    std::string tupltype;

    is >> line;
    is >> channels;


    is >> line;
    is >> maxval;

    is >> line;
    is >> tupltype;

    is >> line; // ENDHDR


    img.resize(height, width);
    std::cout << width << "x" << height << std::endl;

    for(size_t x=0;x<img.rows();x++){
        for(size_t y=0;y<img.cols();y++){
            img(x, y) = is.get();   
        }
    }
    is.close();
}


int main(int argc, char** argv){
    if(argc < 2){
        std::cerr << "Wrong number of parameters.\nUsage: combine <filename>\n";
        return EXIT_FAILURE;
    }
    std::string in_file(argv[1]);
    std::string input = in_file.substr(0, in_file.size()-4);
    std::cout << input << std::endl;
    mat<uint8_t> R;
    mat<uint8_t> G;
    mat<uint8_t> B;
    
    load_pam(input+"_R.pam", R);
    load_pam(input+"_G.pam", G);
    load_pam(input+"_B.pam", B);

    mat<vec3b> RGB(R.rows(), R.cols());
    for(int r=0;r<RGB.rows();++r){
        for(int c=0;c<RGB.cols();++c){
            RGB(r, c)[0] = R(r, c);
            RGB(r, c)[1] = G(r, c);
            RGB(r, c)[2] = B(r, c);
        }
    }
    save_pam(input+"_reconstructed.pam", RGB);
    return EXIT_SUCCESS;

}