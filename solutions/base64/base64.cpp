#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include "base64.h"
#include "base64.h"
namespace {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
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

std::string base64_decode(const std::string& input) {
    std::string output;
    int i = 0;
    int j = 0;
    int in_len = input.size();
    unsigned char char_array_4[4], char_array_3[3];
    while (in_len-- && (input[j] != '=') && is_base64(input[j])) {
        char_array_4[i++] = input[j++];
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++)
                output += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (int k = i; k < 4; k++)
            char_array_4[k] = 0;

        for (int k = 0; k < 4; k++)
            char_array_4[k] = base64_chars.find(char_array_4[k]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (int k = 0; k < i - 1; k++)
            output += char_array_3[k];
    }

    return output;
}



enum class Operation {
    Encode,
    Decode,
};

void PrintUsage() {
    std::cout << "Usage: program_name [encode|decode] file_path\n";
}

Operation GetOperation(const std::string& arg) {
    if (arg == "encode")
        return Operation::Encode;
    else if (arg == "decode")
        return Operation::Decode;
    else
        throw std::runtime_error("Invalid operation");
}

std::string ReadFileContent(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

void EncodeFile(const std::string& file_path) {
    std::string file_content = ReadFileContent(file_path);
    std::string encoded_content = Base64Encode(file_content);

    std::cout << "Base64 encoded content:\n" << encoded_content << std::endl;
}

void DecodeFile(const std::string& file_path) {
    std::string file_content = ReadFileContent(file_path);
    std::string decoded_content = base64_decode(file_content);

    std::cout << "Base64 decoded content:\n" << decoded_content << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        PrintUsage();
        return 1;
    }

    try {
        Operation operation = GetOperation(argv[1]);
        std::string file_path = argv[2];

        switch (operation) {
            case Operation::Encode:
                EncodeFile(file_path);
                break;
            case Operation::Decode:
                DecodeFile(file_path);
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
