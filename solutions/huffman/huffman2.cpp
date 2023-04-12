#include <iostream>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include "../helper/bit_writer.h"
#include "../helper/bit_reader.h"
#define MAGIC_NUMBER "HUFFMAN2"

using namespace std;

struct HuffNode{
    char sym_;
    uint32_t freq_;
    HuffNode* left_;
    HuffNode* right_;

    HuffNode(char sym, uint32_t freq){
        sym_ = sym;
        freq_ = freq;
    }
    HuffNode(char sym, uint32_t freq, HuffNode* left, HuffNode* right){
        sym_ = sym;
        left_ = left;
        right_ = right;
    }
    

};

// comprare operator
struct Compare {
    bool operator() (HuffNode* a, HuffNode* b) {
        return a->freq_ > b->freq_;
    }
};
struct HuffCode{
    uint32_t code_;
    uint32_t len_;
    HuffCode(const uint32_t code, const uint32_t len){
        code_ = code;
        len_ = len;
    }
};

struct HuffEntry{
    char sym_;
    HuffCode *code_;

    HuffEntry (){
        sym_ = 0;
        code_ = nullptr;
    }
    HuffEntry(const char sym, HuffCode* code){
        sym_ = sym;
        code_ = code;
    }

};


HuffNode* buildHuffTree(const std::unordered_map<char,uint32_t>& freq){

    std::priority_queue<HuffNode*,std::vector<HuffNode*>,Compare> pq;
    for(auto& p:freq){
        pq.push(new HuffNode(p.first,p.second));
    }
        while(pq.size() > 1){
            auto v1 = pq.top();

            pq.pop();
            auto v2 = pq.top();
            pq.pop();

            pq.push(new HuffNode('$',v1->freq_+v2->freq_,v1, v2));
        }
        
        return pq.top();
}


void generateCodes(HuffNode* tree, std::unordered_map<char,HuffEntry>& codes, uint32_t code, uint32_t len){
    if(!tree->left_){
        codes[tree->sym_] = *new HuffEntry(tree->sym_,new HuffCode(code,static_cast<uint32_t>(log2(len)+1)));
    }
        else{
            generateCodes(tree->left_, codes, code << 1, len+1);
            generateCodes(tree->right_, codes, (code << 1) | 1, len+1);
        }
}

void huffmanEncode(const std::string& fin, const std::string& fout){

    std::ifstream is(fin, std::ios::binary);
    std::ofstream os(fout, std::ios::binary);
    if(!is){
        std::cerr << "\nError opening the file: " << fin << std::endl;
        return;
    }
    if(!os){
        std::cerr << "\nError opening the file: " << fout << std::endl;
        return;
    }
    // read bytes and compute frequencies
    char c;
    uint32_t num_symbols = 0;
    std::unordered_map<char,uint32_t> freq;
    while(is.read(&c,1)){
        freq[c]++;
        num_symbols++;
    }
    HuffNode* tree = buildHuffTree(freq);
    std::unordered_map<char,HuffEntry> huff_table;
    generateCodes(tree, huff_table,0,0);
    vector<pair<char,HuffEntry>> canon(huff_table.begin(), huff_table.end());
    // sort the table in ascending order
    std::sort(canon.begin(), canon.end(), [](const auto& a, const auto& b){return (a.second.code_->len_ != b.second.code_->len_) ? a.second.code_->len_ < b.second.code_->len_:a.first < b.first;});
    // covert to canonical codes
    uint32_t len = 1;
    uint32_t code = -1;
    for(auto& c:canon){
        if(c.second.code_->len_ > len){
            code = (code + 1) << (c.second.code_->len_ - len);
            c.second.code_->code_ = code;
            len = c.second.code_->len_;

        }
        else {
            // only other possible case is len == cur_len
            code++;
            c.second.code_->code_ = code;
        }
    }

    // write the codes' table
    std::cout << MAGIC_NUMBER << std::endl;;
    std::cout << huff_table.size() << std::endl;;
    bitwriter bw(os);
    os << MAGIC_NUMBER;
    os.put(static_cast<char>(huff_table.size()));
    cout << "-------\n";
    for(auto& entry :huff_table){
        
        std::cout  << "symbol: " << entry.first << '|';
        std::cout <<  "len: " << entry.second.code_->len_ << '|';
        std::cout << "code: " << entry.second.code_->code_<< std::endl;
        bw.write(entry.first,8);
        bw.write(entry.second.code_->len_, 5);
    }
    cout << "-------\n";

    bw.write(num_symbols,32);
    is.clear();
    is.seekg(0);
    while(1){
        int c = is.get();
        if(c == EOF)
            break;
        bw(huff_table[c].code_->code_,huff_table[c].code_->len_);
        std::cout << huff_table[c].code_->code_;
    }
    std::cout << std::endl;
    is.close();
    os.close();

}

void huffmanDecode(const std::string& input, const std::string& output)
{
	using namespace std;
	ifstream is(input, ios::binary);
	if (!is) {
		cerr << "Cannot open input file\n";
	}

	string MagicNumber(8,' ');
	raw_read(is, MagicNumber[0], 8);
	if (MagicNumber != MAGIC_NUMBER) {
		cerr << "Wrong input format\n";
	}

	size_t TableEntries = is.get();
	if (TableEntries == 0) {
		TableEntries = 256;
	}

	bitreader br(is);
	struct entry {
		uint32_t sym_;
		uint32_t len_;
		uint32_t code_;

		bool operator<(const entry& rhs) const {

			return (len_ != rhs.len_)?len_ < rhs.len_:sym_ < rhs.sym_;
		}
	};
	vector<entry> table;
    uint32_t code = -1, len=1;
	for (size_t i = 0; i < TableEntries; ++i) {
		entry e;
		e.sym_ = br.read(8);
		e.len_  = br.read(5);
		if(e.len_ > len){
            code = (code + 1) << (e.len_ - len);
            len = e.len_;
        }else{
            code++;
        }
        e.code_ = code;
		table.push_back(e);
	}
	sort(begin(table), end(table));

	uint32_t NumSymbols;
	NumSymbols  = br.read(32);

	ofstream os(output, ios::binary);
	if (!os) {
		perror("Cannot open output file\n");
	}

	for (size_t i = 0; i < NumSymbols; ++i) {
        uint32_t bit=0,len=0,code=0;
        bool found = 0;
        while(1){
            if(found || is.eof())
                break;                
            // read 1 bit
            bit = br.read(1);
            code = code << 1 | bit;
            len++;
            //std::cout << code << endl;
            for(auto& cur:table){
                //std::cout << cur.code_<< ' ';
                if(cur.code_ == code && cur.len_ == len){
                    
                    os.put(cur.sym_);
                    found = !found;
                    break;
                }
            }
            //std::cout << endl;
        }

    }
    is.close();
    os.close();
}


int main(int argc, char** argv){
    if(argc != 4){
        std::cerr << "\nSyntax error.\nUsage: huffman2 [c/C|d/D] <input_file> <output_file>\n";
        return EXIT_FAILURE;
    }
    char option = argv[1][0];
    if(option == 'c' || option == 'C'){
        // encode
        huffmanEncode(argv[2], argv[3]);
    }
    else if(option == 'd' || option == 'D'){
        // decode
        huffmanDecode(argv[2], argv[3]);
    }
    else{
        std::cerr << "\nSyntax error.\nUsage: huffman2 [c/C|d/D] <input_file> <output_file>\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}