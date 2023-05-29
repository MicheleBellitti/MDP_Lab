#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <iterator>

using namespace std;

class elem
{
private:
    /* data */
    int32_t id_, size_;
    char* data_;
public:
    elem();
    elem(int32_t id, int32_t size, char* data);
    ~elem();
    elem read_el(ifstream& is);
};

elem::elem()
{
    id_ = 0;
    size_ = 0;
    data_ = nullptr;
}
elem::elem(int32_t id, int32_t size, char* data)
{
    id_ = id;
    size_ = size;
    data_ = &data[0];
}

elem::read_el(ifstream& is){
    if(!is)
        exit(1);
    elem val;

    return val;
}

elem::~elem()
{
    delete elem::data_;
}

