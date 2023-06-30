#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include "ppm.h"

void PackBitsEncode(const mat<uint8_t> &img, std::vector<uint8_t> &encoded)
{
    uint8_t prev = 0;
    int32_t run = 0;

    for (int i = 0; i < img.size(); i++)
    {
        uint8_t cur = img.data()[i];
        if (cur == prev)
        {
            run++;

            continue;
        }
        else
        {
            if (run < 2)
            {
                int offset = 1;
                while (cur != prev && i + offset < img.size())
                {

                    offset++;
                }
                if (offset > 2)
                {
                    offset -= 2;
                }
                encoded.push_back(offset);
                for (int j = i - 1; j - i < offset; j++)
                {
                    encoded.push_back(img.data()[j]);
                }
                std::cout << offset;
            }
            else
            {
                std::cout << (257 - run) << ',' << cur << std::endl;
                encoded.push_back(257 - run);
                encoded.push_back(cur);
                run = 0;
            }
        }
    }

    encoded.push_back(128);
}
std::string Base64Encode(std::vector<uint8_t> &v)
{

    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    std::unordered_map<int32_t, char> table;
    int count = 0;
    for (const char &c : base64_chars)
    {
        table[count++] = c;
    }
    std::string s;
    char b3[3];
    for (int i = 0; i < v.size(); i++)
    {
        b3[0] = v[i++];
        b3[1] = b3[i++];
        b3[2] = b3[i++];

        char b4[4];

        b4[0] = (b3[0] & 0xfc) >> 2;
        b4[1] = ((b3[0] & 0x03) >> 4) + ((b3[1] & 0xf0) >> 4);
        b4[2] = ((b3[1] & 0x0f) << 2) + ((b3[2] & 0xc0) >> 6);
        b4[3] = (b3[2] & 0x3f);

        for (int j = 0; j < 4; j++)
        {
            b4[j] = b4[j] < 0 ? 0 : b4[j] > 64 ? 64
                                               : b4[j];
            s += base64_chars[b4[j]];
        }
    }

    if (s.size() % 3 != 0)
    {
        while (s.size() % 3 != 0)
        {
            s += '=';
        }
    }
    return s;
}
