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
/*
int main() {
    // Specify the file path
    std::string file_path = "file01.b64";

    // Read the file content
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return 1;
    }
    std::string file_content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    // Decode the file content using base64_decode function
    std::string decoded_content = base64_decode(file_content);

    // Print the decoded content
    std::cout << "Decoded content:\n" << decoded_content << std::endl;

    return 0;
}*/