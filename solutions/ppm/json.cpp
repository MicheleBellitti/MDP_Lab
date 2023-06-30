#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "ppm.h"
//#include "ppm_utils.h"



std::string JSON(const std::string &filename)
{
    mat<vec3b> img;
    LoadPPM(filename, img);
    mat<uint8_t> r(img.rows() , img.cols()), g(img.rows() , img.cols()), b(img.rows() , img.cols());

    SplitRGB(img, r, g, g);
    std::vector<uint8_t> red, blue, green;


    PackBitsEncode(r, red);
    PackBitsEncode(g, green);
    PackBitsEncode(b, blue);

    std::string out("{\n\t");

    out += "\"width\": " + std::to_string(img.cols());

    out += ",\n\t";
    out += "\"height\": " + std::to_string(img.rows());

    out += ",";


    // write the rgb fields
    out += "\n\t\"red\": \"" + Base64Encode(red) + "\"";
    out += ",\n\t\"green\": \"" + Base64Encode(green) + "\"";
    out += ",\n\t\"blue\": \"" + Base64Encode(blue) + "\"";
    out += "\n}";
    return out;
}
/* 
 int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "SYNTAX: process_ppm <filename>.ppm\n";
        return EXIT_FAILURE;
    }

    std::string filename(argv[1]);

    mat<vec3b> img;
    std::string json = JSON(filename);

    std::cout << json << std::endl;

    return EXIT_SUCCESS;
}   */