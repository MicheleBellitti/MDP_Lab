#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <bitset>

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
            generateCodes(tree->right_, codes, code << 1 | 1, len+1);
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
	if (MagicNumber != "HUFFMAN1") {
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
			return len_ < rhs.len_;
		}
	};
	vector<entry> table;
	for (size_t i = 0; i < TableEntries; ++i) {
		entry e;
		br.read(e.sym_, 8);
		br.read(e.len_, 5);
		br.read(e.code_, e.len_);
		table.push_back(e);
	}
	sort(begin(table), end(table));

	uint32_t NumSymbols;
	br.read(NumSymbols, 32);

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
            br.read(bit,1);
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