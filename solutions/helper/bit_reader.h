#ifndef BITREADER_H
#define BITREADER_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

template<typename T>
std::istream &raw_read(std::istream &is, T &val, size_t size = sizeof(T));

class bitreader {
    uint8_t buffer_;
    uint8_t n_ = 0;
    std::istream& is_;

public:
    bitreader(std::istream &is);

    uint32_t read_bit();

    uint32_t read(uint8_t n);

    uint32_t operator()(uint8_t n);

    bool fail() const;

    explicit operator bool() const;
};

#endif // BITREADER_H
