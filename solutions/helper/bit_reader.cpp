#include "bit_reader.h"

template<typename T>
std::istream &raw_read(std::istream &is, T &val, size_t size) {
	return is.read(reinterpret_cast<char *>(&val), size);
}


uint32_t bitreader::read_bit() {
	if (n_ == 0) {
		raw_read(is_, buffer_);
		n_ = 8;
	}
	--n_;
	return (buffer_ >> n_) & 1;
}

uint32_t bitreader::read(uint8_t n) {
	uint32_t u = 0;
	while (n --> 0) {
		u = (u << 1) | read_bit();
	}
	return u;
}

uint32_t bitreader::operator()(uint8_t n) {
	return read(n);
}

bool bitreader::fail() const {
	return is_.fail();
}
 bitreader::operator bool() const {
	return !fail();
}
