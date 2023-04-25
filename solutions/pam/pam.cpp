#include "../helper/matrix.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <algorithm>

void saveRGBPAM(const std::string& filename, Matrix& image_data){

    std::ofstream os(filename+".pam", std::ios::binary);

    // header for rgb
    os.write("P7\n",3);
    os.write("WIDTH ",6);
    os.put(image_data.cols());
    os.put('\n');
   os.write("HEIGHT ",7);
    os.put(image_data.rows());
    os.put('\n');
    os.write("DEPTH 3\n", 8);
    os.write("MAXVAL 255\n", 11);
    os.write("TUPLTYPE RGB\n", 19);
    os.write("ENDHDR\n", 7);

    // write raster
    for(size_t i=1;i<image_data.rows();i++){
        for(size_t j=0;j<image_data.cols();j++){
            os.put(image_data(i, j).r);
            os.put(image_data(i, j).g);
            os.put(image_data(i, j).b);
        }
    }

    os.close();

}

/*void saveGreyscalePAM(const std::string& filename, Matrix& image_data){

    std::ofstream os(filename+".pam", std::ios::binary);

    // header for rgb
    os.write("P7\n",3);
    os.write("WIDTH ",6);
    os.put(image_data.cols());
    os.put('\n');
   os.write("HEIGHT ",7);
    os.put(image_data.rows());
    os.put('\n');
    os.write("DEPTH 1\n", 8);
    os.write("MAXVAL 255\n", 11);
    os.write("TUPLTYPE GREYSCALE\n", 19);
    os.write("ENDHDR\n", 7);

    // write raster
    for(size_t i=1;i<image_data.rows();i++){
        for(size_t j=0;j<image_data.cols();j++){
            os.put(image_data(i, j).r);
            os.put(image_data(i, j).g);
            os.put(image_data(i, j).b);
        }
    }

    os.close();

}
*/

auto load_pam(const std::string& filename){
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


    Matrix frog(height, width);
    std::cout << width << "x" << height << std::endl;

    for(size_t x=0;x<frog.rows();x++){
        for(size_t y=0;y<frog.cols();y++){
            frog(x, y).r = is.get();
            frog(x, y).g = is.get();
            frog(x, y).b = is.get();
        }
    }
    is.close();
    return frog;
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
        for(size_t i=0;i<data.rows();i++){

                //data[i].assign(data[i].size(), i);
        }

        //saveGreyscalePAM("grey_level",data);

    }
    

    /**
     * @brief EX 2
     * Write a program that opens the "frog.pam" file and creates a "flippad" (upside down) version, that is, 
     * the first line at the top becomes the last at the bottom of the new image, 
     * the second becomes the penultimate and so on. 
     * Save the image in PAM format. Verify that the image is viewable in XnView.
     */
    else if(strcmp(argv[1], "flip") == 0){

        Matrix frog = load_pam("frog");
        // flip vertically the image
        frog.hflip();
        // save the flipped image
        //saveGreyscalePAM("frog-flipped", frog);
    }



    /**
     * @brief EX 3
     * Write a program that opens the "laptop.pam" file and creates a "mirrored" version, 
     * that is, the first column on the left becomes the last on the right of the new image, 
     * the second becomes the penultimate and so on. 
     * Save the image in PAM format. Verify that the image is viewable in XnView.
     */
    else if(strcmp(argv[1], "mirror") == 0){
        std::cout << "loading image.." << std::endl;
        Matrix img = load_pam("laptop");
        // flip horizontally the image
        std::cout << "flipping image" << std::endl;
        img.hflip();
        // save the flipped image
        std::cout << "saving image.." << std::endl;
        saveRGBPAM("laptop-mirrored", img);
        std::cout << "program ended." << std::endl;

    }
    else{

        std::cerr << "\nError: wrong argument\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;





}