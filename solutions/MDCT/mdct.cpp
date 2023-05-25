#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <cmath>
#include <math.h>
#define USE_MAT_DEFINES;
using namespace std;

vector<int16_t> compute_MDCT(int& N, vector<int16_t>& samples, vector<vector<double>>& table){
    vector<int16_t> coeffs(N, 0);

    for(int k=0;k<N;k++){
        double xk = 0;
        for(int n=0;n<2*N;++n){
            double wn = sin(0.5*M_PI*(n+0.5)/N);
            xk += samples[n]*wn*table[k][n];
        }
        coeffs[k] = static_cast<int16_t>(xk);
    }
    // Print the read samples
    // for (const auto& sample : coeffs) {
    //     std::cout << sample << " ";
    // }

    return coeffs;
}

vector<int16_t> compute_IMDCT(int& N, vector<int16_t>& cosines, vector<vector<double>>& table){

    vector<int16_t> coeffs(2*N, 0);

    for(int n=0;n<N;n++){
        
        double wn = sin(0.5*M_PI*(n+0.5)/N);
        double x = 2*wn/N;
        for(int k=0;k<2*N;++k){
            
            x += cosines[n]*table[k][n];
        }
        coeffs[n] *= static_cast<int16_t>(x);
    }

    return coeffs;
}

bool get_samples(const std::string& filename, std::vector<int16_t>& samples) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error opening the input file\n";
        return false;
    }

    // Determine the size of the file
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    // Calculate the number of int16_t values in the file
    std::size_t numSamples = fileSize / sizeof(int16_t);

    // Resize the samples vector
    samples.resize(numSamples);

    // Read the data directly into the vector
    inputFile.read(reinterpret_cast<char*>(samples.data()), fileSize);

    inputFile.close();

    // Print the read samples
    // for (const auto& sample : samples) {
    //     std::cout << sample << " ";
    // }
    std::cout << std::endl;

    return true;
}


double entropy(vector<int16_t>& samples){
    map<int16_t, int16_t> freq;
    // computet the frequencies
    for(int16_t& v: samples){
        freq[v]++;
    }

    double e = 0;
    for(auto [sample, fr]: freq){
        e += (fr)*log2(fr);
    }
    return log2(samples.size()) - e/samples.size();
}

int main(){
    const string infile("test.raw");


    vector<int16_t> samples;
    cout << "get_samples()...called\n";
    get_samples(infile, samples);
    cout << "entropy()...called\n";
    double e = entropy(samples);
    cout << "entropy()...computed\n";
    vector<int16_t> quantized(samples.begin(), samples.end());
    ofstream quant_os("quantized_qt.raw", std::ios::binary);
    cout << "quantized_qt.raw...saved\n";
    for(int16_t& quant: quantized){
        quant /= 2600;
    }
    cout << "quantize()...done\n";

    double quant_entropy = entropy(quantized);
    
    cout << "test.raw Entropy: " << e << endl;

    cout << "quantized test.raw Entropy: " << quant_entropy << endl;

    // dequantize the data and save it
    for(int16_t& quant: quantized){
        quant *= 2600;
    }
    quant_os.write(reinterpret_cast<char*>(quantized.data()), 2* quantized.size());
    quant_os.close();
    ofstream err_os("error_qt.raw", std::ios::binary);
    
    for(int i = 0;i<samples.size();i++){
        int error = samples[i] - quantized[i];
        err_os.put(error);
    }
    err_os.close();

    /**
     * @brief 
     * At this point you can use Audacity by loading the dequantized and the error signals: 
     * listening to them together you have the original signal,
     * listening to them separately you can hear the result of the quantization or the error.
     * 
     */

    int N = 1024;
    // compute the cosine's matrix
    
    vector<vector<double>> table(N, vector<double>(2*N, 0));

    for(int i = 0;i<N;i++){
        for(int j=0;j<2*N;j++){
            table[i][j] = cos((M_PI/N)*(j+0.5+0.5*N)*(i+0.5));
        }
    }
    auto cosines = compute_MDCT(N, samples, table);

    // quantize
    for(auto& cosine: cosines){
        cosine /= 1e4;
    }

    quant_entropy = entropy(cosines);

    // dequantize

    for(auto& cosine: cosines){
        cosine *= 1e4;
    }

    // save the dequantized signal
    ofstream os("output.raw", std::ios::binary);
    os.write(reinterpret_cast<char*>(cosines.data()), cosines.size()*2);
    os.close();

    ofstream err("error.raw", std::ios::binary);
    
    for(int i = 0;i<samples.size();i++){
        int error = samples[i] - cosines[i];
        err.put(error);
    }
    err.close();

    return EXIT_SUCCESS;


}