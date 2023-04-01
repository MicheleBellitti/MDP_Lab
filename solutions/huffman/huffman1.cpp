#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

template<typename T>
std::istream& raw_read(std::istream& is, T& num, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&num), size);
}
template<typename T>
std::ostream& raw_write(std::ostream& os, const T& num, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<const char*>(&num), size);
}

class bitreader {
	std::istream& is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit() {
		if (nbits_ == 0) {
			raw_read(is_, buffer_);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

public:
	bitreader(std::istream& is) : is_(is), nbits_(0) {}

	std::istream& read(uint32_t& u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}

	std::istream& read(int32_t& i, size_t n) {
		uint32_t u;
		read(u, n);
		i = static_cast<int32_t>(u);
		return is_;
	}
};

class bitwriter {
	std::ostream& os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u) {
		// buffer_ = buffer_ * 2 + u % 2;
		buffer_ = (buffer_ << 1) | (u & 1);
		++nbits_;
		if (nbits_ == 8) {
			raw_write(os_, buffer_);
			nbits_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os), nbits_(0) {}

	~bitwriter() {
		flush();
	}

	std::ostream& write(uint32_t u, size_t n) {
		while (n-- > 0) {
			write_bit(u >> n);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		return write(u, n);
	}

	void flush(uint32_t u = 0) {
		while (nbits_ > 0) {
			write_bit(u);
		}
	}
};

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
        return a->freq_ >= b->freq_;
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


HuffNode* buildHuffTree(const std::map<char,uint32_t>& freq){

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


void generateCodes(HuffNode* tree, std::map<char,HuffEntry>& codes, uint32_t code, uint32_t len){
    if(!tree->left_){
        codes[tree->sym_] = *new HuffEntry(tree->sym_,new HuffCode(code,len));
    }
        else{
            generateCodes(tree->left_, codes, code << 1|1, len+1);
            generateCodes(tree->right_, codes, code << 1, len+1);
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
    std::map<char,uint32_t> freq;
    while(is.read(&c,1)){
        freq[c]++;
        num_symbols++;
    }
    HuffNode* tree = buildHuffTree(freq);
    std::map<char,HuffEntry> huff_table;
    generateCodes(tree, huff_table,0,0);

    // write the codes' table
    std::cout << "HUFFMAN1" << std::endl;;
    std::cout << huff_table.size() << std::endl;;
    bitwriter bw(os);
    os << "HUFFMAN1";
    os.put(static_cast<char>(huff_table.size()));

    for(auto& entry :huff_table){
        
        std::cout  << "symbol: " << entry.first << '|';
        std::cout <<  "len: " << entry.second.code_->len_ << '|';
        std::cout << "code: " << entry.second.code_->code_<< std::endl;
        bw.write(entry.first,8);
        bw.write(entry.second.code_->len_, 5);
        bw.write(entry.second.code_->code_, entry.second.code_->len_);
    }
    bw.write(num_symbols,32);
    is.clear();
    is.seekg(0);
    while(1){
        int c = is.get();
        if(c == EOF)
            break;
        bw(huff_table[c].code_->code_,huff_table[c].code_->len_);
    }
    is.close();
    os.close();


}

void huffmanDecode(const std::string& input_filename, const std::string& output_filename)
{
    // Open input file
    std::ifstream input_file(input_filename, std::ios::binary);
    if (!input_file.is_open()) {
        throw std::runtime_error("Failed to open input file");
    }

    // Read magic number
    const std::string expected_magic_number = "HUFFMAN1";
    std::string magic_number(8, ' ');
    raw_read(input_file, magic_number[0], 8);
    if (magic_number != expected_magic_number) {
        throw std::runtime_error("Wrong input format");
    }
    // Read Huffman table
    const size_t num_table_entries = input_file.get() ? input_file.get() : 256;
    bitreader br(input_file);
    std::vector<HuffEntry> table;
    table.reserve(num_table_entries);
    for (size_t i = 0; i < num_table_entries; ++i) {
        HuffEntry e;
        input_file.read(&e.sym_,1);
        br.read(e.code_->len_, 5);
        br.read(e.code_->code_, e.code_->len_);
        table.push_back(e);
    }
    std::sort(std::begin(table), std::end(table), 
	[] (const auto& lhs, const auto& rhs) {
    return lhs.code_->len_ < rhs.code_->len_;
});
	input_file.close();
    // Read number of symbols
    uint32_t num_symbols = 0;
    br.read(num_symbols, 32);

    // Open output file
    std::ofstream output_file(output_filename, std::ios::binary);
    if (!output_file.is_open()) {
        throw std::runtime_error("Failed to open output file");
    }

    // Decode symbols
    for (size_t i = 0; i < num_symbols; ++i) {
        uint32_t len = 0, code = 0;
        size_t pos = 0;
        while (pos < table.size()) {
            while (table[pos].code_->len_ > len) {
                uint32_t bit;
                if (!br.read(bit, 1)) {
                    throw std::runtime_error("Failed to read input file");
                }
                code = (code << 1) | bit;
                ++len;
            }
            if (code == table[pos].code_->code_) {
                output_file.put(table[pos].sym_);
                break;
            }
            ++pos;
        }
        if (pos == table.size()) {
            throw std::runtime_error("Symbol not found in Huffman table");
        }
    }
	output_file.close();
}
int main(int argc, char** argv){
    if(argc != 4){
        std::cerr << "\nSyntax error.\nUsage: huffman1 [c/C|d/D] <input_file> <output_file>\n";
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
        std::cerr << "\nSyntax error.\nUsage: huffman1 [c/C|d/D] <input_file> <output_file>\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}