#include "pbm.h"
#include <cmath>
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
    H = h;
    W = w; 
    BitReader br(is);
    
    ImageData = std::vector<uint8_t>((int)std::floor((h+7)/8)*w);
    is.read(reinterpret_cast<char*>(ImageData.data()), ImageData.size());
    
    std::cout << "size: " << ImageData.size() << std::endl;

    return true;
}

Image BinaryImageToImage(BinaryImage& bimg)
{
    int W, H;
    W = bimg.W;
    H = bimg.H;
    Image img;
    img.ImageData = std::vector<uint8_t>(bimg.ImageData.size());
    


    for(int r=0;r<img.ImageData.size();r++){
        img[r] = bimg[r] == 0?0:255;
    }
    return img;

}

// int main(int argc, char** argv){
//     if(argc < 2)
//         return EXIT_FAILURE;
//     std::string filename(argv[1]);
//     BinaryImage img;
//     img.ReadFromPBM(filename);

//     std::cout << img.W_;
//     Image converted(img.W_, img.H_);
    

//     converted.W_ = img.W_;
//     converted.H_ = img.H_;
//     converted = BinaryImageToImage(img);

//     for(int i=0;i<converted.H_;i++){
//         for(int j=0;j<converted.W_;++j){
//             std::cout << (int)converted[i*converted.H_+j] << "|";
//         }
//     }

//     return EXIT_SUCCESS;
// }