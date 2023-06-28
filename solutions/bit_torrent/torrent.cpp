#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <iomanip>

struct elem {
	char type_; // 's': string, 'i': int, 'l':list, 'd':dictionary
	std::string s_;
	int64_t i_;
	std::vector<elem> l_;
	std::map<std::string, elem> d_;

	elem() : type_(0) {}

	elem(std::istream &is) {
		char end;
		switch (is.peek()) {
		break; case 'i': is >> type_ >> i_ >> end;
		break; case 'l':
			is >> type_;
			while (is.peek() != 'e') {
				l_.emplace_back(is);
			}
			is >> end;
		break; case 'd':
			is >> type_;
			while (is.peek() != 'e') {
				elem key(is);
				d_[key.s_] = elem(is);
			}
			is >> end;
		break; default:
			type_ = 's';
			size_t len;
			char cln;
			is >> len >> cln;
			s_.resize(len);
			is.read(&s_[0], len);
		}
	}

	void print(size_t tab) const 
	{
		switch (type_) {
		break; case 's':
		std::cout << '"';
			for (const auto &x : s_) {
				std::cout << (31 < x && x < 127 ? x : '.');
			}
			std::cout << '"';			
		break; case 'i':
			std::cout << i_;
		break; case 'l':
			std::cout << "[";
			for (const auto &x : l_) {
				std::cout << "\n";
				std::cout << std::string(tab + 1, '\t');
				x.print(tab + 1);
			}
			std::cout << "\n";
			std::cout << std::string(tab, '\t');
			std::cout << "]";
		break; case 'd':
			std::cout << "{";
			for (const auto &x : d_) {
				std::cout << "\n";
				std::cout << std::string(tab + 1, '\t');
				std::cout << '"' << x.first  << '"' << " => ";
				if (x.first == "pieces") {
					const auto &p = x.second.s_;
					std::cout << std::hex << std::setfill('0');
					for (size_t i = 0; i < p.size(); ++i) {
						if (i % 20 == 0) {
							std::cout << "\n";
							std::cout << std::string(tab + 1, '\t');
						}
						//printf("%02x", p[i]);
						std::cout << std::setw(2) << int(char(p[i]));
					}
					std::cout << std::dec;
				}
				else {
					x.second.print(tab + 1);
				}
			}
			std::cout << "\n";
			std::cout << std::string(tab, '\t');
			std::cout << "}";
		break; default:;
		}
	}
};

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		std::cout << "SYNTAX: " << argv[0] << "<file .torrent>\n";
		return EXIT_FAILURE;
	}

	std::ifstream is(argv[1], std::ios::binary);
	if (!is) {
		std::cout << "Cannot open " << argv[1] << "\n";
		return EXIT_FAILURE;
	}

	elem root(is);
	root.print(0);

	return 0;
}