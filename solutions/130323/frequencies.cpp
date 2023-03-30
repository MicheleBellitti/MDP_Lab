#include <iostream>
#include<fstream>
#include <map>

using namespace std;

int main(int argc, char ** argv){
if(argc != 3){
    cerr << "Wrong number of arguments!" << endl;
    return EXIT_FAILURE;
}
string fin(argv[1]);
string fout(argv[2]);

map<int, long long> map;

ifstream is(fin, ios::binary);
while(1){

    int c;
    c = is.get();
    if(c == EOF)
        break;
    map[c]++;
}

is.close();
ofstream os(fout);
for(auto& p: map){
    os << hex << setw(2) << setfill('0') << uppercase << p.first;
			os << '\t' << dec << p.second << endl;
}
os.close();
return EXIT_SUCCESS;
}