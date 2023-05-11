
#include<iostream>
#include <array>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include "mat.h"

bool save_pam(const std::string& filename, const mat<uint8_t>& img)
{
	std::ofstream os(filename+".pam", std::ios::binary);
	if (!os)
		return false;

	os << "P7" << "\n";
	os << "WIDTH " << img.cols();
    os.put('\n');
    os << "HEIGHT " << img.rows();
    os.put('\n');
    os << "DEPTH 1\n";
    os << "MAXVAL 255\n";
    os << "TUPLTYPE GRAYSCALE\n";
    os << "ENDHDR\n";

		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				os.put(img(r, c));
                
			}
		}

	return true;
}

bool load_pam_split(const std::string& filename, mat<uint8_t>& img_R, mat<uint8_t>& img_G, mat<uint8_t>& img_B)
{
	using namespace std;
	ifstream is(filename+".pam", ios::binary);
	if(!is){
        std::cerr << "Error opening the pam file: " << filename << ".pam" << std::endl;
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

    std::cout << width << 'x' << height << std::endl;
	img_R.resize(height, width);
    img_G.resize(height, width);
    img_B.resize(height, width);
    
    for (int r = 0; r < img_R.rows(); ++r) {
			for (int c = 0; c < img_R.cols(); ++c) {
				
                

                img_B(r, c) = is.get();
                img_R(r, c) = is.get();
                img_G(r, c) = is.get();

                


			}
    }

	return true;
}



int main(int argc, char** argv){
    if(argc < 2){
        std::cerr << "Wrong number of parameters.\nUsage: split <filename>.pam\n";
        return EXIT_FAILURE;
    }
    std::string in_file(argv[1]);
    std::string input = in_file.substr(0, in_file.size()-4);

    mat<uint8_t> R;
    mat<uint8_t> G;
    mat<uint8_t> B;

    if (load_pam_split(input, R, G, B)){
        std::cout << "pam file read correctly.\n";
        save_pam(input+"_R", R);
        save_pam(input+"_G", G);
        save_pam(input+"_B", B);
        return EXIT_SUCCESS;
    }
    else return EXIT_FAILURE;
    
}