#include "pbm.h"
bool BinaryImage::ReadFromPBM(const std::string& filename){

    std::ifstream is(filename);
    if(!is){
        std::cerr << "Error opening the input file\n";
        return false;
    }


    // header
    std::string magicNumber;
    int32_t w, h;
    std::string line;

    is >> magicNumber;
    if(magicNumber != "P4"){
        std::cerr << "Magic number is wrong\n";
        return false;
    }
    is >> line;
    if(!line.starts_with('#')){
        // not a comment [<W> <H>\n]
        w = atoi(line.c_str());
        is >> h;
    }
    else{

        is >> w;
        is >> h;
    }
    std::cout << w << ' ' << h << std::endl;
    
    BitReader br(is);
    
    ImageData = std::vector<uint8_t>((int)std::floor((h+7)/8)*w);
    H = h;
    W = w;
    is.read(reinterpret_cast<char*>(ImageData.data()), ImageData.size());
    
    std::cout << "size: " << ImageData.size() << std::endl;
    return true;
}

//   int main(int argc, char** argv){
//      if(argc < 2)
//          return EXIT_FAILURE;
//      std::string filename(argv[1]);
//      BinaryImage img;
//      img.ReadFromPBM(filename);
//      return EXIT_SUCCESS;
//  }