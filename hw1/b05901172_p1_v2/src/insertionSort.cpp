#include "parser.h"
#include "parser.cpp"
#include <vector>
#include <fstream>

typedef pair<string,size_t> word;

int main(int argc,char** argv)
{
    AlgParser* parser=new AlgParser;
    parser->Parse(string(argv[1]));
    vector<word> data;
    size_t size=parser->QueryTotalStringCount();
    data.reserve(size);
    for(size_t i=0;i<size;++i){
        data.push_back( make_pair(parser->QueryString(i),i) );
    }
    delete parser;


    for(size_t i=1;i<size;++i){
        size_t k=0;
        for(int j=i-1;j>=0;--j){
            if( !(data[i].first<data[j].first) ){
                k=j+1;
                break;
            }
        }
        word temp=data[i];
        for(size_t j=i;j>k;--j){
            data[j]=data[j-1];
        }
        data[k]=temp;
    }
    
    ofstream ofs(argv[2]);
    ofs<<size;
    for(size_t i=0;i<size;++i){
        ofs<<endl<<data[i].first<<" "<<data[i].second+1;
    }
}