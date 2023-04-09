#ifndef BITWRITER_H
#define BITWRITER_H

#include <iostream>
#include <cstdint>

template<typename T>
std::ostream &raw_write(std::ostream &os, const T &val, size_t size = sizeof(T));

class bitwriter {
    uint8_t buffer_;
    int n_ = 0;
    std::ostream& os_;

    std::ostream &write_bit(uint32_t bit);

public:
    bitwriter(std::ostream& os);

    std::ostream &write(uint32_t u, uint8_t n);

    std::ostream &operator()(uint32_t u, uint8_t n);

    std::ostream &flush(uint32_t bit = 0);

    ~bitwriter();
};

#endif // BITWRITER_H
