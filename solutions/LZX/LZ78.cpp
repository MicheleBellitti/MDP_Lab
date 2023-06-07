#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <iterator>

// Command line program to encode and decode a file with the LZ78 algorithm

using namespace std;

void error(bool input, const string &filename)
{

    cerr << "Error opening " << (input ? "input" : "output") << " file: " << filename << endl;

    return;
}

void syntax()
{
    cerr << "Wrong number of parameters:\nUsage: lz78 [c/d] <input file> <output file>\nTry again!\n";
}

vector<pair<int, char>> read_compressed(ifstream &is)
{
    vector<pair<int, char>> data;
    while (true)
    {
        int b1 = is.get();
        char b2;
        is.read(&b2, 1);

        data.push_back(pair<int, char>{b1, b2});
    }

    return data;
}

vector<pair<int, char>> compress(const vector<char> &input)
{
    unordered_map<string, int> dictionaryMap;
    string currentSequence;
    vector<pair<int, char>> compressedData;

    int index = 0;
    for (char c : input)
    {
        currentSequence += c;
        if (dictionaryMap.count(currentSequence) == 0)
        {
            dictionaryMap[currentSequence] = ++index;
            compressedData.emplace_back(dictionaryMap[currentSequence.substr(0, currentSequence.length() - 1)], c);
            currentSequence = "";
        }
    }

    if (!currentSequence.empty())
    {
        compressedData.emplace_back(dictionaryMap[currentSequence], '\0');
    }

    return compressedData;
}
string decompress(const vector<pair<int, char>> &compressedData)
{
    unordered_map<int, string> dictionary;
    string decompressedData;
    int index = 0;

    for (const auto &entry : compressedData)
    {
        if (entry.first == 0)
        {
            decompressedData += entry.second;
            dictionary[++index] = string(1, entry.second);
        }
        else
        {
            string sequence = dictionary[entry.first] + entry.second;
            decompressedData += sequence;
            dictionary[++index] = sequence;
        }
    }

    return decompressedData;
}

int main(int argc, char **argv)
{
    // Usage: LZ78 [c|d|a] <input_f> <output_f>
    if (argc < 4)
    {
        syntax();
        return EXIT_FAILURE;
    }
    const char option = argv[1][0];
    const string infile(argv[2]);
    const string outfile(argv[3]);
    ifstream is(infile);

    if (option == 'c')
    {
        is.seekg(0, ios::end);
        int size = is.tellg();
        is.seekg(0, ios::beg);
        vector<char> input(size, 0);

        for_each(input.begin(), input.end(), [&is](char &v)
                 { is.read(&v, 1); });

        is.close();
        ofstream os(outfile, ios::binary);
        vector<pair<int, char>> compressedData = compress(input);

        cout << "Compressed data: ";
        for (const auto &entry : compressedData)
        {
            cout << "(" << entry.first << ", " << entry.second << ") ";
            os.put(entry.first);
            os.put(entry.second);
        }
        cout << endl;
    }
    else if (option == 'd')
    {
        // decompress command
        ifstream is(infile);

        // I should build the compress data from the compressed file :p
        auto data = read_compressed(is);
        is.close();
        string decompressedData = decompress(data);
        ofstream os(outfile, ios::binary);
        cout << "Decompressed data: " << decompressedData << endl;
        os << "Decompressed data: " << decompressedData;
        os << decompressedData;
    }
    else
    {
        vector<char> input(*istream_iterator<char>(is), *istream_iterator<char>());
        is.close();
        ofstream os(outfile, ios::binary);
        vector<pair<int, char>> compressedData = compress(input);

        cout << "Compressed data: ";
        for (const auto &entry : compressedData)
        {
            cout << "(" << entry.first << ", " << entry.second << ") ";
            os.put(entry.first);
            os.put(entry.second);
        }
        cout << endl;
        auto decompressed = decompress(compressedData);
        cout << "Decompressed data: " << decompressed << endl;
        os << "Decompressed data: " << decompressed;
        os << decompressed;
        os.close();
    }

    return EXIT_SUCCESS;
}
