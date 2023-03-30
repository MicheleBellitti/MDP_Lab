#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <bitset>
#include "bit_reader.cpp"
#include "bit_writer.cpp"
using namespace std;



uint8_t count_bits(int k){
    uint8_t count = 0;
    while(k > 0){
        ++count;
        k /= 2;
    }
    return count;
}
void encode(ifstream& is, ofstream& os){
    // FUNZIA!!
    
    // map numbers from Z to N
    vector<int> v{
        istream_iterator<int>(is),
        istream_iterator<int>()
    };

    // mapping
    for(int& i: v){
        if(i < 0){
            i = 2*abs(i);
        }
        else{
            i = 2*i+1;
        }
    }
    for(int& k: v){
        
       // encode the number and write it
       int N = floor(log2(static_cast<double>(k)));
       uint8_t j = count_bits(k);
       bitset<32> bits(k);
       bits = (bits << 32 - N - j);
       cout << k << ' ' << bits.to_string().substr(0, N+j) << endl;
       for(char& c : bits.to_string().substr(0, N+j)){
            os.write(&c, 1);
       }

    }
}

void decode(ifstream& is, ofstream& os){
    //SIAMO DAVVERO VICINI ALLA SOLUZIONE! MANCA POCO!!

    // count the number of zeros read
    int count = 0;
    uint8_t n;
    //bitreader br(is);
    uint8_t bit = 0;
    bool found = false;
    int bits_size = 0;
  while (is.good() && !is.eof()) {
    is.read(reinterpret_cast<char*>(&n),1);
    bit = (bit << 1) | n;
    if (n == 0 && !found) {
        // If we find a zero and we're not in the middle of decoding,
        // we need to start counting zeros.
        count++;
        //cout << "first" << ' ' << count << endl;
       
    } 
    if (n == 1 && !found) {
        // If we find a one and we're in the middle of counting zeros,
        // we need to start reading the actual bits.
        //cout << "zeros finished!" << ' ' << count << endl;
        
        found = true;
        
    }
    if (found) {
        // If we finished counting zeros, we have to read count bits.
        //cout << "second" << ' ' << n << endl;
        bits_size++;
        count--;
        
    }
    if (count == 0 || bits_size == 32) {
        // At this point we either read all the count bits of we reached the capacity for a $ bytes integer,
        // thus we need to write the value read to the output file and then flush our buffer.
        //cout << "third" << ' ' << bit << endl;
        int value = 0;
        /*if(bit % 2 == 0){
            value  = - bit / 2;
        }
        else{
            value = bit / 2;
        }*/
        os << bit << endl;
        bit = 0;
        count = bits_size = 0;
        found = false;  
    }
}

}


int main(int argc, char** argv){
        if(argc != 4){
            cerr << "Wrong number of parameters.\nUsage: elias [c|d] <filein> <fileout>\n";
            return EXIT_FAILURE;
        }
        char option = argv[1][0];
        
        if(option == 'c'){
            ifstream is(argv[2]);
            ofstream os(argv[3], ios::binary);
            encode(is,os);
        }
        else{
            ifstream is(argv[2], ios::binary);
            ofstream os(argv[3]);
            decode(is,os);
        }

        return EXIT_SUCCESS;
}