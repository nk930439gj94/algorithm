#include "parser.h"
#include "parser.cpp"
#include <vector>
#include <fstream>

void mergesort(AlgParser*,vector<size_t>&,size_t,size_t);
void merge(AlgParser*,vector<size_t>&,size_t,size_t,size_t);

vector<size_t> temp;

int main(int argc,char** argv)
{
    AlgParser* parser=new AlgParser;
    parser->Parse(string(argv[1]));
    vector<size_t> data;
    size_t size=parser->QueryTotalStringCount();
    data.reserve(size);
    temp.reserve(size);
    for(size_t i=0;i<size;++i){
        data.push_back(i);
    }

    mergesort(parser,data,0,size);

    ofstream ofs(argv[2]);
    ofs<<size;
    for(size_t i=0;i<size;++i){
        ofs<<endl<<parser->QueryString(data[i])<<" "<<data[i]+1;
    }
    delete parser;
}

void mergesort(AlgParser* p,vector<size_t>& v, size_t head, size_t tail){
    if( head != (head+tail)/2 )
        mergesort( p, v, head, (head+tail)/2 );
    if( ((head+tail)/2)+1 != tail )
        mergesort( p, v, (head+tail)/2, tail );
    if( head+1 != tail )
        merge( p, v, head, (head+tail)/2, tail );
}

void merge( AlgParser* p, vector<size_t>& v, size_t a, size_t b, size_t d ){
    temp.clear();
    size_t c=b;
    size_t begin=a;
    while(true){
        if( a==b && c==d ) break;
        else if( a==b ){
            for(size_t i=c;i<d;++i){
                temp.push_back(v[i]);
            }
            break;
        }
        else if( c==d ){
            for(size_t i=a;i<b;++i){
                temp.push_back(v[i]);
            }
            break;
        }
        else if( p->QueryString(v[c])<p->QueryString(v[a]) ){
            temp.push_back(v[c++]);
        }
        else{
            temp.push_back(v[a++]);
        }
    }
    for(size_t i=begin;i<d;++i){
        v[i]=temp[i-begin];
    }
}