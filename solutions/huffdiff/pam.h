#pragma once

#include "mat.h"
#include <array>
#include <string>

using rgb = std::array<uint8_t, 3>;

bool save_pam(const mat<uint8_t> &img, const std::string &filename);
bool load_pam(mat<uint8_t> &img, const std::string &filename);

bool save_pam(const mat<rgb> &img, const std::string &filename);
bool load_pam(mat<rgb> &img, const std::string &filename);