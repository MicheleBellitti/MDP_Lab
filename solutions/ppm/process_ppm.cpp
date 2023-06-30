#include <iostream>
#include <fstream>
#include <cmath>
#include "ppm.h"
#include <sstream>

void SplitRGB(const mat<vec3b> &img, mat<uint8_t> &img_r, mat<uint8_t> &img_g, mat<uint8_t> &img_b)
{
    if (img.size() == 0)
    {
        return;
    }
    img_r.resize(img.rows(), img.cols());
    img_g.resize(img.rows(), img.cols());
    img_b.resize(img.rows(), img.cols());

    for (size_t r = 0; r < img.rows(); r++)
    {
        for (size_t c = 0; c < img.cols(); c++)
        {
            img_r = img(r, c)[0];
            img_g = img(r, c)[1];
            img_b = img(r, c)[2];
        }
    }
}

bool LoadPPM(const std::string &filename, mat<vec3b> &img)
{
    std::ifstream is(filename);

    if (!is)
    {
        std::cerr << "Error opening the input file " << filename << std::endl;
        return false;
    }
    std::string magic, line;

    std::vector<std::string> lines;
    while (std::getline(is, line))
    {
        if (line[0] != '#')
        {
            lines.emplace_back(line);
            // std::cout << line;
        }
    }

    // std::cout << linestream.str() << std::endl;
    magic = lines[0];
    if (magic != "P6")
    {
        std::cout << magic;
        std::cerr << "wrong magic number!\n";
        return false;
    }

    int W, H;
    std::istringstream w_h(lines[1]);
    
    w_h >> W >> H;

    if (W == 0 || H == 0)
    {
        std::cerr << "there was an error while parsing the header..\n";
        return false;
    }

    // maxval

    int64_t maxval;
    maxval = atoi(lines[2].c_str());
    std::cout << "maxval: " << maxval << std::endl;

    if (maxval <= 0 || maxval > 65536)
    {
        std::cerr << "there was an error while parsing the header..\n";
        return false;
    }

    // read data
    // 1 byte for each sample if maxval < 256
    // 2 bytes per sample otherwise

    img.resize(H, W);

    for (size_t r = 0; r < H; r++)
    {
        for (size_t c = 0; c < W; c++)
        {
            is >> img(r, c)[0] >> img(r, c)[1] >> img(r, c)[2];
        }
    }

    return true;
}

// int main(int argc, char **argv)
// {
//     if (argc < 2)
//     {
//         std::cerr << "SYNTAX: process_ppm <filename>.ppm\n";
//         return EXIT_FAILURE;
//     }

//     std::string filename(argv[1]);

//     mat<vec3b> img;

//     LoadPPM(filename, img);

//     std::cout << img.rows() << ',' << img.cols() << std::endl;

//     return EXIT_SUCCESS;
// }