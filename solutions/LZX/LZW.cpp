#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

void encodeLZW(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }

    if (!output.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    // Build the initial dictionary with single character entries
    std::map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, static_cast<char>(i))] = i;
    }

    std::string current;
    char symbol;
    int nextCode = 256;

    while (input.get(symbol)) {
        std::string currentPlusSymbol = current + symbol;
        if (dictionary.count(currentPlusSymbol)) {
            current = currentPlusSymbol;
        } else {
            // Write the code for the current string to the output file
            output << dictionary[current] << " ";

            // Add the new string to the dictionary
            dictionary[currentPlusSymbol] = nextCode++;
            current = symbol;
        }
    }

    // Write the code for the last string to the output file
    output << dictionary[current] << " ";

    input.close();
    output.close();
}

void decodeLZW(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }

    if (!output.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    // Build the initial dictionary with single character entries
    std::map<int, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    int code;
    std::string current;
    int nextCode = 256;

    while (input >> code) {
        std::string entry;
        if (dictionary.count(code)) {
            std::cout << code << std::endl;
            entry = dictionary[code];
        } else if (code == nextCode) {
            entry = current + current[0];
        } else {
            std::cerr << "Invalid code: " << code << std::endl;
            return;
        }

        // Write the entry to the output file
        output << entry;

        // Add the new entry to the dictionary
        dictionary[nextCode++] = current + entry[0];

        current = entry;
    }

    input.close();
    output.close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: ./LZW <input_file> <output_file> <operation>" << std::endl;
        std::cout << "Operation: encode | decode | both" << std::endl;
        return 0;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string operation = argv[3];

   if (operation == "encode" || operation == "both") {
    encodeLZW(inputFile, outputFile + ".encoded");
    std::cout << "LZW encoding completed." << std::endl;
}

if (operation == "decode" || operation == "both") {
    decodeLZW(outputFile + ".encoded", outputFile + ".decoded");
    std::cout << "LZW decoding completed." << std::endl;
}


    return 0;
}
