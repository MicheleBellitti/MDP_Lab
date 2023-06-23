#include "lzs.h"
#include "../helper/bit_reader.h"

void lzs_decompress(std::ifstream& is, std::ostream& os)
{
    if(!is || !os){
        std::cerr << "Error opening input or output files\n";
        return;
    }
    bitreader br(is);
    std::string buffer;
    std::string out;

    // I try to read bits directly from stdin
    while(is.good()){
        uint8_t bit = br.read_bit();
        if(br.fail())
            break;
        
        buffer = out.substr(out.size()- (out.size()>= 2000?2000:out.size()), out.size()>= 2000?2000:out.size());
        std::cout << "buffer: " << buffer << std::endl;
        if(bit == 1){
            uint16_t offset = 0, lenght=0;
            // read next bit
            uint8_t next = br.read_bit();
            if(next == 1){
                // offset < 128
                 offset = br.read(7);
                
            }
            else{
                // offset > 128 --> 11 bits
                 offset = br.read(11);
            }

            // lenght

            // some code for getting the lenght right

            // try #1: read two bits at a time and check its value
            bool found = false;
            int size = 0;
            while(true){
                if(size < 4)
                {
                    lenght += br.read(2);
                    size += 2;
                }
                else{
                    int N = 1;
                    while(true){
                        // read 4 bits at a time
                        uint16_t val = br.read(4);
                        if(val == 16){
                            N++;
                            continue;
                            }
                        else{
                            lenght = (N*15-7) + val;
                            break;
                        }
                    }
                }
                if(size == 2 && lenght == 0 || lenght == 1 || lenght == 2){
                    lenght = lenght == 0?2:(lenght == 1?3:4);
                    break;
                }
                if(size == 4 && lenght == 12 || lenght == 13 || lenght == 14){
                    lenght = lenght == 12?5:(lenght == 13?6:7);
                    break;
                }

            }
            std:: cout << "offset: " << offset << " len: " << lenght << std::endl;
            // copy starting from offset
            for(int i=0;i<lenght;i++){
                    out += buffer[buffer.size()-offset+i];
                    
            }
  
        }
        else{
            // literal byte
            uint8_t byte = br.read(8);
            out += byte;
            std::cout << "literal: " << byte << '\n';
        }

    }
    os << out;
}

int main(){
    std::ifstream is("test.txt.lzs");
    lzs_decompress(is, std::cout);
    return 0;
    
}