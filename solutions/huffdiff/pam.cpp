#include "pam.h"

#include <fstream>
#include <sstream>

bool save_pam(const mat<uint8_t> &img, const std::string &filename)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os) {
		return false;
	}
	os << "P7\n";
	os << "WIDTH " << img.cols() << "\n";
	os << "HEIGHT " << img.rows() << "\n";
	os << "DEPTH 1\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE GRAYSCALE\n";
	os << "ENDHDR\n";

	//for (int r = 0; r < img.rows(); ++r) {
	//	for (int c = 0; c < img.cols(); ++c) {
	//		os.put(img(r, c));
	//	}
	//}

	os.write(img.rawdata(), img.rawsize());

	return true;
}

bool load_pam(mat<uint8_t> &img, const std::string &filename)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::string magic_number;
	std::getline(is, magic_number);
	if (magic_number != "P7") {
		return false;
	}

	int w, h;
	while (1) {
		std::string line;
		std::getline(is, line);
		if (line == "ENDHDR") {
			break;
		}
		std::stringstream ss(line);
		std::string token;
		ss >> token;
		if (token == "WIDTH") {
			ss >> w;
		}
		else if (token == "HEIGHT") {
			ss >> h;
		}
		else if (token == "DEPTH") {
			int depth;
			ss >> depth;
			if (depth != 1) {
				return false;
			}
		}
		else if (token == "MAXVAL") {
			int maxval;
			ss >> maxval;
			if (maxval != 255) {
				return false;
			}
		}
		else if (token == "TUPLTYPE") {
			std::string tupltype;
			ss >> tupltype;
			if (tupltype != "GRAYSCALE") {
				return false;
			}
		}
		else {
			return false;
		}
	}

	img.resize(h, w);

	//for (int r = 0; r < img.rows(); ++r) {
	//	for (int c = 0; c < img.cols(); ++c) {
	//		img(r, c) = is.get();
	//	}
	//}
	is.read(img.rawdata(), img.rawsize());

	return true;
}

bool save_pam(const mat<rgb> &img, const std::string &filename)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os) {
		return false;
	}
	os << "P7\n";
	os << "WIDTH " << img.cols() << "\n";
	os << "HEIGHT " << img.rows() << "\n";
	os << "DEPTH 3\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE RGB\n";
	os << "ENDHDR\n";

	//for (int r = 0; r < img.rows(); ++r) {
	//	for (int c = 0; c < img.cols(); ++c) {
	//		os.put(img(r, c)[0]);
	//		os.put(img(r, c)[1]);
	//		os.put(img(r, c)[2]);
	//	}
	//}

	os.write(img.rawdata(), img.rawsize());

	return true;
}

bool load_pam(mat<rgb> &img, const std::string &filename)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::string magic_number;
	std::getline(is, magic_number);
	if (magic_number != "P7") {
		return false;
	}

	int w, h;
	while (1) {
		std::string line;
		std::getline(is, line);
		if (line == "ENDHDR") {
			break;
		}
		std::stringstream ss(line);
		std::string token;
		ss >> token;
		if (token == "WIDTH") {
			ss >> w;
		}
		else if (token == "HEIGHT") {
			ss >> h;
		}
		else if (token == "DEPTH") {
			int depth;
			ss >> depth;
			if (depth != 3) {
				return false;
			}
		}
		else if (token == "MAXVAL") {
			int maxval;
			ss >> maxval;
			if (maxval != 255) {
				return false;
			}
		}
		else if (token == "TUPLTYPE") {
			std::string tupltype;
			ss >> tupltype;
			if (tupltype != "RGB") {
				return false;
			}
		}
		else {
			return false;
		}
	}

	img.resize(h, w);

	//for (int r = 0; r < img.rows(); ++r) {
	//	for (int c = 0; c < img.cols(); ++c) {
	//		img(r, c)[0] = is.get();
	//		img(r, c)[1] = is.get();
	//		img(r, c)[2] = is.get();
	//	}
	//}
	is.read(img.rawdata(), img.rawsize());

	return true;
}

