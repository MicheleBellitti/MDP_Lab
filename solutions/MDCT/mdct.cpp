#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <cmath>
#include <math.h>
#include <algorithm>
using namespace std;

template<typename CT, typename ST>
void win_mdct(const std::vector<ST>& padded, std::vector<CT>& coeffs, size_t offset, size_t N, vector<vector<double> > cos_tab, vector<double> sin_tab)
{
	for (size_t k = 0; k < N; ++k) {
		double Xk = 0.;
		for (size_t n = 0; n < 2 * N; ++n) {
			ST xn = padded[offset*N + n];
			double wn = sin_tab[n];
			double cn = cos_tab[k][n];

			Xk += xn * cn * wn;
		}

		coeffs[offset*N + k] = CT(round(Xk));
	}
}


template<typename CT, typename ST>
std::vector<CT> MDCT(const std::vector<ST>& samples, size_t N, vector<vector<double> > cos_tab, vector<double> sin_tab)
{
    using namespace std;
	auto nwin = size_t(ceil(samples.size() / double(N))) + 2;
	vector<ST> padded(nwin * N, 0);
	copy(begin(samples), end(samples), begin(padded) + N);

	vector<CT> coeffs((nwin - 1) * N);
	for (size_t i = 0; i < nwin - 1; ++i)
		win_mdct(padded, coeffs, i, N, cos_tab, sin_tab);

	return coeffs;
}

template<typename CT>
std::vector<double> win_imdct(const std::vector<CT>& coeffs, size_t offset, size_t N, vector<vector<double> > cos_tab, vector<double> sin_tab)
{
    using namespace std;
    vector<double> recon(2 * N);

	for (size_t n = 0; n < 2 * N; ++n) {
		double tmpn = 0.;
		for (size_t k = 0; k < N; ++k) {
			CT xk = coeffs[offset*N + k];
			double ck = cos_tab[k][n];
			tmpn += ck * xk;
		}

		recon[n] = 2. / N * tmpn;
	}

	return recon;
}

template<typename ST, typename CT>
std::vector<ST> IMDCT(const std::vector<CT>& coeffs, size_t N, vector<vector<double> > cos_tab, vector<double> sin_tab)
{
    using namespace std;
    auto nwin = coeffs.size() / N;
	vector<ST> samples((nwin - 1) * N);

	vector<double> prev = win_imdct(coeffs, 0, N, cos_tab, sin_tab);
	for (size_t i = 1; i < nwin; ++i) {
		vector<double> curr = win_imdct(coeffs, i, N, cos_tab, sin_tab);

		for (size_t j = 0; j < N; ++j)
			samples[(i - 1)*N + j] = ST(round(curr[j] + prev[N + j]));
		
		prev = move(curr);
	}

	return samples;
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
        //cout << sample << " " << fr << endl;
        e -= (fr)*log2(fr);
    }
    
    return log2(samples.size()) + e/samples.size();
}

int main(){
    const string infile("test.raw");


    vector<int16_t> samples;
    cout << "get_samples()...called\n";
    get_samples(infile, samples);
    cout << "entropy()...called\n";
    double e = entropy(samples);
    cout << "entropy()...computed\n";
    vector<int16_t> quantized(samples.size());
    ofstream quant_os("quantized_qt.raw", std::ios::binary);
    cout << "quantized_qt.raw...saved\n";
    std::transform(samples.begin(), samples.end(), quantized.begin(), [](int16_t& v){return v/2600;});
    cout << "quantize()...done\n";

    double quant_entropy = entropy(quantized);
    
    cout << "test.raw Entropy: " << e << endl;

    cout << "quantized test.raw Entropy: " << quant_entropy << endl;

    // dequantize the data and save it
    std::transform(quantized.begin(), quantized.end(), quantized.begin(), [](int16_t& v){return v*2600;});
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
    // compute the sine's and cosine's matrix
    std::vector<double> sin_tab;
	sin_tab.resize(2*N);
	for (size_t i = 0; i < sin_tab.size(); ++i)
		sin_tab[i] = sin((M_PI / (2 * N)) * (i + 0.5));

    vector<vector<double>> table(N, vector<double>(2*N, 0));

    for(int i = 0;i<N;i++){
        for(int j=0;j<2*N;j++){
            table[i][j] = cos((M_PI/N)*(j+0.5+0.5*N)*(i+0.5));
        }
    }
    auto cosines = MDCT<int16_t>(samples, N, table, sin_tab);

    // quantize
    for(auto& cosine: cosines){
        cosine /= 1e4;
    }

    quant_entropy = entropy(cosines);

    cout << "MDCT Entropy: " << quant_entropy << endl;

    // dequantize

    for(auto& cosine: cosines){
        cosine *= 1e4;
    }

    // save the dequantized signal
    cosines = IMDCT<int16_t>(cosines, N, table, sin_tab);
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