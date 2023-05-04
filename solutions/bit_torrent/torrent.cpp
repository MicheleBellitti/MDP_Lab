#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stack>

using namespace std;
std::ofstream os("output.txt");
class BValue{
    public:
        enum Type{
            INT,
            STR,
            LIST,
            DICT
        };
        BValue(Type t):type(t){}
        virtual ~BValue(){}
        Type type;
        virtual void print(int indent_size ){
        
        }
        
};



class DictValue: public BValue{
    public:
        DictValue():BValue(DICT){}
        vector<pair<string, BValue*>> values;
        
        void print(int indent_size){
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << "{" << endl;
            for(auto& p: values){
                for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
                os << p.first << " => ";
                p.second->print(indent_size);
             os << endl;
            }
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << '}' << endl;
        }
};
class ListValue: public BValue{
    public:
        ListValue():BValue(LIST){}
        vector<BValue*> values;
        void print(int indent_size){
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << '[' << endl;
            for(auto& v: values){
                v->print(indent_size);
                os << endl;
            }
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << ']' << endl;
        }
};
class IntValue: public BValue{
    public:
        IntValue():BValue(INT){}
        int value;
        void print(int indent_size){
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << value << endl;
        }
};
class StrValue: public BValue{
    public:
        StrValue():BValue(STR){}
        string value;

        void print(int indent_size){
            for (int i = 0; i < indent_size; i++)
                {
                    os << ' ';
                }
            os << '"' << value << '"' << endl;
        }
};

BValue* get_elem(ifstream& is){
    char c = is.get();
    if(c == 'd'){
        DictValue* d = new DictValue();
        while((c = is.get()) != 'e'){
            is.unget();
            string len;
            char c = is.get();
            while(c != ':'){
                len += c;
                c = is.get();
            }
            string key(stoi(len), 0);
            is.read(&key[0], stoi(len));
            BValue* val = get_elem(is);
            d->values.push_back(make_pair(key, val));
        }
        return d;
    }
    else if(c == 'l'){
        ListValue* l = new ListValue();
        while((c = is.get()) != 'e'){
            is.unget();
            BValue* val = get_elem(is);
            l->values.push_back(val);
        }
        return l;
    }
    else if(c == 'i'){
        string buf;
        while((c = is.get()) != 'e'){
            buf += c;
        }
        IntValue* i = new IntValue();
        i->value = stoi(buf);
        return i;
    }
    else if(isdigit(c) != 0){
        string len;
        len += c;
        while((c = is.get()) != ':'){
            len += c;
        }
        string buf(stoi(len), 0);
        is.read(&buf[0], stoi(len));
        StrValue* s = new StrValue();
        s->value = buf;
        return s;
    }
    else{
        os << "Error" << endl;
        return nullptr;
    }
}

bool read_torrent(ifstream& is,int& indent_size, stack<char>& s, BValue* val = nullptr){
    
        val = get_elem(is);
        val->print(indent_size);
        if (is.peek() == EOF)
        
        
        return read_torrent(is, indent_size, s, val);
        return true;
}


int main(int argc, char** argv){
    
    if(argc != 2){
        os << "Usage: ./torrent <filename>" << endl;
        return EXIT_FAILURE;
    }
    string filename(argv[1]);
    ifstream is(filename + ".torrent", std::ios::binary);
    if(!is){
        os << "Error opening file" << endl;
        return 1;
    }
    stack<char> s;
    int ind = 0;
    bool ret = read_torrent(is, ind, s);
    os << ret << endl;
    return EXIT_SUCCESS;

}