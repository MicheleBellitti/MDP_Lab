#include "../helper/matrix.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <algorithm>
/*
void saveRGBPAM(const std::string& filename, Matrix& image_data){

    std::ofstream os(filename+".pam", std::ios::binary);
    int w=image_data.cols(), h=image_data.rows();
    // header for rgb
    os.write("P7\n",3);
    os << "WIDTH " << w;
    os.put('\n');
    os << "HEIGHT " << h;
    os.put('\n');
    os << "DEPTH 3\n";
    os << "MAXVAL 255\n";
    os << "TUPLTYPE RGB\n";
    os << "ENDHDR\n";

    // write raster
    for(size_t i=0;i<image_data.rows();i++){
        for(size_t j=0;j<image_data.cols();j++){
            os << image_data(i, j).r;
            os << image_data(i, j).g;
            os << image_data(i, j).b;
        }
    }

    os.close();

}
*/
void saveGreyscalePAM(const std::string& filename, Matrix& image_data){
    // function for saving a greyscale image in the PAM format
    std::ofstream os(filename+".pam", std::ios::binary);

    // header for greyscale
    os.write("P7\n",3);
    os << "WIDTH " << image_data.cols();
    os.put('\n');
    os << "HEIGHT " << image_data.rows();
    os.put('\n');
    os << "DEPTH 1\n";
    os << "MAXVAL 255\n";
    os << "TUPLTYPE RGB\n";
    os << "ENDHDR\n";

    // write raster
    for(size_t i=0;i<image_data.rows();i++){
        for(size_t j=0;j<image_data.cols();j++){
        // 1-channel images
            os.put(image_data(i, j));
            
        }
    }

    os.close();

}

/*
void load_pam_rgb(const std::string& filename, Matrix& frog){
    std::ifstream is(filename+".pam",std::ios::binary);
    if(!is){
        std::cerr << "Error opening the pam file: " << filename << ".pam" << std::endl;
        exit(1);
    }
    std::string magicNumber;
    is >> magicNumber;
    if(strcmp(magicNumber.data(),"P7") != 0){
        std::cerr << "Wrong PAM format(magic number incorrect)" << std::endl;
        exit(1);
    }
    size_t width, height;
    std::string line;

    is >> line;
    is >> width;

    is >> line;
    is >> height;


    int channels, maxval;
    std::string tupltype;

    is >> line;
    is >> channels;


    is >> line;
    is >> maxval;

    is >> line;
    is >> tupltype;

    is >> line; // ENDHDR


    frog.resize(height, width);
    std::cout << width << "x" << height << std::endl;

    for(size_t x=0;x<frog.rows();x++){
        for(size_t y=0;y<frog.cols();y++){
            frog(x, y).r = is.get();
            frog(x, y).g = is.get();
            frog(x, y).b = is.get();
        }
    }
    is.close();
}*/

void load_pam_grey(const std::string& filename, Matrix& frog){
    std::ifstream is(filename+".pam",std::ios::binary);
    if(!is){
        std::cerr << "Error opening the pam file: " << filename << ".pam" << std::endl;
        exit(1);
    }
    std::string magicNumber;
    is >> magicNumber;
    if(strcmp(magicNumber.data(),"P7") != 0){
        std::cerr << "Wrong PAM format(magic number incorrect)" << std::endl;
        exit(1);
    }
    size_t width, height;
    std::string line;

    is >> line;
    is >> width;

    is >> line;
    is >> height;


    int channels, maxval;
    std::string tupltype;

    is >> line;
    is >> channels;


    is >> line;
    is >> maxval;

    is >> line;
    is >> tupltype;

    is >> line; // ENDHDR


    frog.resize(height, width);
    std::cout << width << "x" << height << std::endl;

    for(size_t x=0;x<frog.rows();x++){
        for(size_t y=0;y<frog.cols();y++){
            frog(x, y) = is.get();
            
        }
    }
    is.close();
}


int main(int argc, char** argv){
    if(argc < 1){
        return EXIT_FAILURE;
    }
    /**
     * @brief EX 1
     * Write a program that generates a gray level image of 256×256 pixels, 
     * in which the first row is made of 256 zeros, the second one of 256 ones, the third one of 256 values 2 and so on. 
     * Save the image in PAM format. Verify that the image is viewable in XnView. 
     * The images should appear as a gradient from black to white from top to bottom.
     **/
    if (strcmp(argv[1], "grey") == 0)
    {
        Matrix data(256, 256);
        std::cout << "Generating 256x256 greyscale gradient top to bottom..";
        for(size_t i=0;i<data.rows();i++){

                data[i].assign(data[i].size(), i);
        }
        std::cout << "..done!\n";
        std::cout << "Saving image..";
        saveGreyscalePAM("grey_level",data);
        std::cout << "..done!\n";
        std::cout << "Program ended.\n";
    }
    

    /**
     * @brief EX 2
     * Write a program that opens the "frog.pam" file and creates a "flippad" (upside down) version, that is, 
     * the first line at the top becomes the last at the bottom of the new image, 
     * the second becomes the penultimate and so on. 
     * Save the image in PAM format. Verify that the image is viewable in XnView.
     */
    else if(strcmp(argv[1], "flip") == 0){
        std::cout << "Loading image.." << std::endl;
        Matrix frog(1, 1);
        load_pam_grey("frog", frog);
        // flip vertically the image
         std::cout << "Flipping image" << std::endl;
        frog.vflip();
        // save the flipped image
        std::cout << "Saving image.." << std::endl;
        saveGreyscalePAM("frog-flipped", frog);
        std::cout << "Program ended." << std::endl;
        
    }



    /**
     * @brief EX 3
     * Write a program that opens the "laptop.pam" file and creates a "mirrored" version, 
     * that is, the first column on the left becomes the last on the right of the new image, 
     * the second becomes the penultimate and so on. 
     * Save the image in PAM format. Verify that the image is viewable in XnView.
     */
    else if(strcmp(argv[1], "mirror") == 0){
        /*std::cout << "Loading image.." << std::endl;
        Matrix img(1, 1);
        load_pam("laptop",img);
        // flip horizontally the image
        std::cout << "Flipping image" << std::endl;
        img.hflip();
        // save the flipped image
        std::cout << "Saving image.." << std::endl;
        saveRGBPAM("laptop-mirrored", img);
        std::cout << "Program ended." << std::endl;
        */

    }
    else{

        std::cerr << "\nError: wrong argument\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;





}
