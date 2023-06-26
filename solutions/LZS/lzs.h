#pragma once
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>

extern void lzs_decompress(std::istream& is, std::ostream& os);

// class bitreader{
//     std::istream& is_;
//     uint8_t buffer_, n_=0;

//     public:
//         bitreader(std::istream& is);
//         uint8_t read_bit();
//         uint32_t read(size_t nbits);
//         void raw_read(std::istream& is, uint8_t& val);
//         bool fail();
// };