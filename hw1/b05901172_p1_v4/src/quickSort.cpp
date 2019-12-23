#include "parser.h"
#include "parser.cpp"
#include <fstream>
#include <vector>

void quicksort(AlgParser*,vector<size_t>&,size_t,size_t);
size_t partition(AlgParser*,vector<size_t>&,size_t,size_t);

int main(int argc,char** argv)
{
    AlgParser* parser=new AlgParser;
    parser->Parse(string(argv[1]));
    vector<size_t> data;
    size_t size=parser->QueryTotalStringCount();
    data.reserve(size);
    for(size_t i=0;i<size;++i){
        data.push_back(i);
    }

    quicksort( parser, data, 0, size );

    ofstream ofs(argv[2]);
    ofs<<size;
    for(size_t i=0;i<size;++i){
        ofs<<endl<<parser->QueryString(data[i])<<" "<<data[i]+1;
    }
    delete parser;
}

void quicksort( AlgParser* p, vector<size_t>& v, size_t a, size_t b ){
    if( (b-a)<2 ) return;
    size_t middle=partition(p,v,a,b);
    if(middle-a>1)
        quicksort(p,v,a,middle);
    if(b-middle>1)
        quicksort(p,v,middle,b);
}

size_t partition( AlgParser* p, vector<size_t>& v, size_t a, size_t b ){
    const string ref= p->QueryString(v[a]);
    while(true){
        --b;
        while( p->QueryString( v[b] ) > ref ) { --b; }
        while( p->QueryString( v[a] ) < ref ) { ++a; }
        if( a<b ){
            size_t temp=v[a];
            v[a++]=v[b];
            v[b]=temp;
        }
        else return b+1;
    }
}