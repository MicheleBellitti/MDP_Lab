#include <iostream>
#include<fstream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

int main(int argc, char ** argv){
    if(argc != 3){
        cerr << "Wrong number of arguments!" << endl;
        return EXIT_FAILURE;
    }

    ifstream is(argv[1],ios::binary);
    vector<int32_t> v;
	while(1){
		int32_t val;
		if(is.eof())
			break;
		is.read(reinterpret_cast<char*>(&val),4);
		v.push_back(val);
	}
is.close();

ofstream os(argv[2]);
for(const auto& x: v){
	os << x << endl;
}
os.close();

}