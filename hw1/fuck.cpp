#include <algorithm>
#include <fstream>
#include "parser.h"
#include "parser.cpp"
using namespace std;

int main(int argc,char** argv)
{
    vector< pair<string,size_t> > data;
    AlgParser parser;
    parser.Parse(argv[1]);
    size_t size=parser.QueryTotalStringCount();
    data.reserve(size);
    for(size_t i=0;i<size;++i){
        data.push_back( make_pair( parser.QueryString(i), i ) );
    }
    
    sort( data.begin(), data.end() );

    ofstream ofs(argv[2]);
    ofs<<size;
    for(size_t i=0;i<size;++i){
        ofs<<endl<<data[i].first<<" "<<data[i].second+1;
    }
}