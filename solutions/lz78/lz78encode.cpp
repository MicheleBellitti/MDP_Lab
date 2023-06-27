#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>

template <typename T>
std::ostream &raw_write(std::ostream &os, const T &val, size_t size = 1UL)
{
    return os.write(reinterpret_cast<const char *>(&val), size);
}

class bitwriter
{

    int8_t n_ = 0, buffer_;
    std::ostream &os_;

public:
    std::ostream &write_bit(uint32_t bit)
    {
        buffer_ = (buffer_ << 1) | (bit & 1);
        ++n_;
        if (n_ == 8)
        {
            raw_write(os_, buffer_);
            n_ = 0;
        }
        return os_;
    }

    bitwriter(std::ostream &os) : os_(os) {}

    std::ostream &write(uint32_t u, uint8_t n)
    {
        for (int i = n - 1; i >= 0; --i)
        {
            write_bit(u >> i);
        }
        return os_;
    }

    std::ostream &operator()(uint32_t u, uint8_t n)
    {
        return write(u, n);
    }

    std::ostream &flush(uint32_t bit)
    {
        while (n_ > 0)
        {
            write_bit(bit);
        }
        return os_;
    }

    ~bitwriter()
    {
        os_.flush();
    }
};

bool lz78encode(const std::string &input_filename, const std::string &output_filename, int maxbits)
{
    std::ifstream is(input_filename, std::ios::binary);
    if (!is)
        return false;
    int max_index = std::pow(2, maxbits) - 1;
    std::ofstream os(output_filename, std::ios::binary);
    if (!os)
        return false;

    bitwriter bw(os);
    std::map<std::string, int> dict;
    std::string text, buff;
    std::vector<std::pair<int, char>> out;
    uint32_t index = 0;
    while (true)
    {
        is >> text;
        if(is.eof())
            break;
        // std::cout << text << std::endl;

        // write the encoded file
        os << "LZ78"; // magic number
        bw.write(maxbits, 5);
        std::string last_occur;
        for (size_t i = 0; i < text.size(); ++i)
        {
            if (dict.size() == max_index + 1){
                dict.clear();
                index = 0;
                std::cout << "[clear] ";
                }
            buff += text[i];
            // std::cout << buff << std::endl;

            if (dict.find(buff) != dict.end() && dict.size() > 0)
            {
                // we try to find a bigger occurrence
                last_occur = buff;
                continue;
            }
            index = last_occur != "" ? dict[last_occur] : index;
            if (buff.size() == 1 && dict.find(buff) == dict.end())
            {
                index = 0;
            }

            // out.push_back({index, buff[buff.size() - 1]});
            bw.write(index, dict.size() > 0 ? (int)std::floor(std::log2(dict.size()) + 1) : 0);
            bw.write(buff[buff.size() - 1], 8);
            std::cout << "(" << index << ',' << buff[buff.size() - 1] << ") ";
            //std::cout << "\nmaxdim: " << dict.size() << std::endl;
            dict[buff] = dict.size() + 1;
            buff.clear();
        }
        if (buff.size() > 0)
        {
            // we need to encode last characters read
            // out.push_back({dict[buff.substr(0, buff.size() - 1)], buff[buff.size() - 1]});
            bw.write(dict[buff.substr(0, buff.size() - 1)], (int)std::floor(std::log2(dict.size())));
            bw.write(buff[buff.size() - 1], 8);
        }
    }

    return true;
}

// int main(int argc, char **argv)
// {
//     if (argc < 3)
//     {
//         std::cerr << "Wrong number of parameters.\nUsage: lz78 <input_filename> <output_filename>\n";
//         return EXIT_FAILURE;
//     }

//     std::string in_file(argv[1]), out_file(argv[2]);
//     return !lz78encode(in_file, out_file, 2);
// }