#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>

using vec3b = std::array<uint8_t, 3>;

template <typename T>
class mat
{
    int rows_, cols_;
    std::vector<T> data_;

public:
    mat(int rows = 0, int cols = 0)
    {
        resize(rows, cols);
    }

    void resize(int rows, int cols)
    {
        assert(rows >= 0 && cols >= 0);
        rows_ = rows;
        cols_ = cols;
        data_.resize(rows * cols);
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    T &operator()(int r, int c)
    {
        assert(r >= 0 && c >= 0 && r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }
    const T &operator()(int r, int c) const
    {
        assert(r >= 0 && c >= 0 && r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }

    T *data() { return data_.data(); }
    const T *data() const { return data_.data(); }
    int size() const { return rows_ * cols_; }

    char *rawdata() { return reinterpret_cast<char *>(data_.data()); }
    const char *rawdata() const { return reinterpret_cast<const char *>(data_.data()); }
    int rawsize() const { return rows_ * cols_ * sizeof(T); }

    auto begin() { return data_.begin(); }
    auto begin() const { return data_.begin(); }
    auto end() { return data_.end(); }
    auto end() const { return data_.end(); }

    template <typename Iter>
    void assign(Iter first, Iter last)
    {
        data_.assign(first, last);
    }
};

void load_data(mat<uint16_t> &pgm, std::istream &is)
{

    is.read(reinterpret_cast<char *>(pgm.rawdata()), pgm.rawsize());
}


mat<uint8_t> load_pgm(std::string filename)
{

    std::ifstream is(filename);
    if (!is)
    {
        std::cerr << "Error opening input file\n";
        exit(1);
    }
    std::string magicNumber;
    is >> magicNumber;
    if (magicNumber != "P5")
    {
        exit(1);
    }
    int width, height, maxval;
    is >> width >> height >> maxval;
    mat<uint16_t> pgm;
    mat<uint8_t> pgm_new;

    pgm.resize(height, width);
    pgm_new.resize(height, width);

    if (maxval < 256)
    {
        is.read(reinterpret_cast<char *>(pgm.rawdata()), pgm.size());
    }
    else
    {
     is.read(reinterpret_cast<char *>(pgm.rawdata()), pgm.rawsize());   
    }

    for(int ind = 0;ind < pgm.size();ind++){
        pgm_new.data()[ind] = pgm.data()[ind];
        if(maxval > 256)
            pgm_new.data()[ind] /= 255; 
    }
    return pgm_new;
}


bool save_pgm(std::string &fout, mat<uint8_t> &img)
{
    fout.append("_gray.pgm");
    

    std::ofstream os(fout);
    if (!os)
    {
        std::cerr << "Error opening output file\n";
        return false;
    }

    os << "P5\n";
    os << img.cols() << ' ' << img.rows() << std::endl;
    os << 255 << std::endl; // maxval

    os.write(img.rawdata(), img.rawsize());
    os.close();
    return true;
}
bool save_ppm(std::string fout, mat<vec3b>& ppm)
{
    fout.append("_bayer.ppm");
    std::ofstream os(fout);

    for(size_t i=0;i<ppm.rows();i++){
        for(size_t j=0;j<ppm.cols();++j){
            if((i % 2 == 0 && j % 2 == 0)){
                // red
                ppm(i, j) = {255, 0 ,0};
            }
            else if((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)){
                // green
                ppm(i, j) = {0, 255, 0};
            }
            else{
                // blue
                ppm(i, j) = {0, 0, 255};
            }
        }
    }
    os << "P6\n";
    os << ppm.cols() << ' ' << ppm.rows() << std::endl << 255 << std::endl;

    for(size_t i=0;i<ppm.rows();i++){
        for(size_t j=0;j<ppm.cols();++j){
            os.write(reinterpret_cast<char*>(&ppm(i, j)[0]), 3);
        }
    }
    os.close();
    return true;

}

void bayer_decode(std::string filename, mat<vec3b>& img){
    

    // first pass: compute the green component for red and blue

    for(size_t i=2;i<img.rows()-2;i++){
        for(size_t j=2;j<img.cols()-2;++j){

        }
    }

    // save green reconstructed image
    save_ppm(filename.append("_green.ppm"), img);

    // second pass: compute the remainign color channels values still using interpolation

    for(size_t i=2;i<img.rows()-2;i++){
        for(size_t j=-2;j<img.cols()-2;++j){


        }
    }

    // save final interpolated image

    save_ppm(filename.append("_interp.ppm"), img);

    return;

    
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "SYNTAX: bayer_decode <input file> <output prefix>\n";
        return EXIT_FAILURE;
    }
    std::string infile(argv[1]);
    std::string outfile(argv[2]);
    // load pgm
    mat<uint8_t> img = load_pgm(infile);

    // save pgm

    save_pgm(outfile, img);

    //save ppm
    mat<vec3b> rgb(img.rows(), img.cols());
    save_ppm(outfile, rgb);
    //bayer_decode(outfile, rgb);

    return EXIT_SUCCESS;
}