#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

// Command line program to encode and decode a file with the LZ78 algorithm

using namespace std;

void error(bool input, const string& filename){

    cerr << "Error opening" << (input? "input":"output") << " file: " << filename << endl;

    return;

}

void syntax(){
    cerr << "Wrong number of parameters:\nUsage: lz78 [c/d] <input file> <output file>\nTry again!\n";
}

bool encode(const string& fin, const string& fout,map<int, string> table){

    ifstream is(fin, std::ios::binary);

    if(!is){
        error(true, fin);
        return false;
    }

    ofstream os(fout, std::ios::binary);


    if(!os){
        error(false, fin);
        return false;
    }
    
    // table should be of size 0 by default
    int index = 0;
    vector<int> buffer(128);
    while(is.good()){
        is.read(reinterpret_cast<char*>(buffer.data()), 128);

        int actual_size = is.gcount();
        if(actual_size == 0)
            break;
            for(int i=0;i<128;i++){
                int byte = buffer[i];
                // the first char is just copied into the table
                if(i == 0){
                    table[++index] = to_string(char(byte));
                    os.put(table.size());
                    os.put(char(byte));
                }
                else{
                    // Here I have to search for the longest occurrence into the table..
                    string wdw(char(byte));
                    // #1 -> start from the bottom => longest occurrencies guaranteed -> not doable(dict)
                    // #2 -> just keep track of the lenghs and update it only when I find a better match..
                    int ind=0, len=0;
                    for(auto& entry: table){
                            // k,v -> ind,string
                            int id;
                            if(id=(entry.second.find(wdw, 0) )!= std::string::npos){
                                if(len < entry.second.size()-id){
                                    len = entry.second.size()-id;
                                    ind = entry.first;
                                }
                            else{
                                break;
                            }
                            }
                    }

                }
            }
        }

    return true;
}