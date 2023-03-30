#include <iostream>
#include<fstream>
#include <iterator>
#include <vector>
using namespace std;

int main(int argc, char ** argv){
if(argc != 3){
    cerr << "Wrong number of arguments!" << endl;
    return EXIT_FAILURE;
}
string fin(argv[1]);
string fout(argv[2]);

ifstream is(fin);
ofstream os(fout, ios::binary);
vector<int32_t> nums{
    istream_iterator<int32_t>(is),
    istream_iterator<int32_t>()
};

is.close();
for(int i: nums){
    os.write(reinterpret_cast<const char*>(&i), 4);
}
os.close();
return EXIT_SUCCESS;
}