# pragma once
#include "ppm.h"

void PackBitsEncode(const mat<uint8_t> &img, std::vector<uint8_t> &encoded);
void SplitRGB(const mat<vec3b> &img, mat<uint8_t> &img_r, mat<uint8_t> &img_g, mat<uint8_t> &img_b);

bool LoadPPM(const std::string &filename, mat<vec3b> &img);
std::string Base64Encode(std::vector<uint8_t> &v);
