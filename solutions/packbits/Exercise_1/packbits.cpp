#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int packbits_compress(const string& fin, const string& fout)
{
    ifstream is(fin, ios::binary);
    ofstream os(fout, ios::binary);
    if(!is){
        cout << "Error: cannot open input file: " << fin << endl;
        return 1;
    }
    if(!os){
        cout << "Error: cannot open output file: " << fout << endl;
        return 1;
    }
    vector<char> buf(128);
    int run  = 0;
    while (true) {

        // read up to buf.size() bytes into buf
        is.read(&buf[0], buf.size());
        int n = is.gcount();

        // check if any bytes were read
        if (n == 0) {
            break;
        }
        char prev = -1;

        for(int i=0;i<n;i++){
            char cur = buf[i];

            if(prev == -1 || cur == prev){

                run++;
            }
            else{
                if(run < 2){
                    /*cout << "i: " << i << endl;
                    cout << buf[i] << endl;*/
                    int offset = 1;
                    while(cur != prev && i+offset < n){
                        prev = cur;
                        cur = buf[i+offset];
                        offset++;
                    }
                    if(offset > 2)
                        offset -= 2;
                    os.put(static_cast<char>(offset));
                    //cout << offset << endl;
                    for(int j=i-1;j-i<offset;j++){
                        os.put(buf[j]);
                    }
                    
                    i += offset;
                    /*cout << "i: " << i << endl;
                    cout << buf[i] << endl;*/
                }
                else{
                    os.put(static_cast<const char>(257-run));
                    os.put(prev);
                }
                run = 1;
            }
            prev = cur;
            
        }

        // check if we have a run left over
        /*if (run > 0) {
            if (run < 2) {
                os.put(static_cast<char>(n - 1));
                for (int j = n - run; j < n; j++) {
                    os.put(buf[j]);
                }
            } else {
                os.put(static_cast<const char>(257 - run));
                os.put(prev);
            }
        }*/
        if(is.eof())
                break;
    }

    is.close();
    os.put(0x80);
    os.close();
    return 0;
}
int packbits_decompress(const string& fin, const string& fout)
{
    ifstream is(fin, std::ios::binary);
    ofstream os(fout, std::ios::binary);
    if(!is){
        cerr << "Error opening the input file: " << fin << endl;
        return 1;
    }
    if(!os){
        cerr << "Error opening the output file: " << fout << endl;
        return 1;
    }
    unsigned int command;
    while((command = is.get()) != EOF){
        if(command == 128)
            break;
        //cout << "command: " << (int)command << endl;

        if(command < 128){
            // write the next command+1 bytes as they are (no encoding)
            for(int i=0;i<command+1;i++){
                char byte2 = is.get();
                if(byte2 == EOF)
                    break;
                os.put(byte2);
            }

        }else if( command > 128){
            
            // repeat command
            char byte2 = is.get();
            for(int i=0;i<257-command;i++){
                os.put(byte2);
            }
        //cout << "byte2: " << (int)byte2 << endl;
        }
    }
    is.close();
    os.close();
    return 0;
}

int main(int argc, char** argv){
    if(argc != 4){
        cerr << "\n Usage: packbits [c|d] <input_file> <output_file>\n";
        return 1;
    }
    char option = argv[1][0];
    if(option  == 'c' || option == 'C')
        return packbits_compress(argv[2], argv[3]);
    else if(option == 'd' || option == 'D'){
        return  packbits_decompress(argv[2], argv[3]);
    }else{
        cerr << "\n Wrong option provided.\n It should be one of these: [c/C|d/D]. Try again\n";
        return 1;
    }
}