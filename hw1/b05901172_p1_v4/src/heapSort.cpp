#include "parser.h"
#include "parser.cpp"
#include <fstream>
#include <vector>


int main(int argc,char** argv)
{
    AlgParser parser;
    vector<size_t> data;
    parser.Parse(string(argv[1]));
    size_t size=parser.QueryTotalStringCount();
    data.reserve(size);
    for(size_t i=0;i<size;++i){
        data.push_back(i);
        size_t pos=i;
        while(pos>0){
            if( parser.QueryString(data[pos])<parser.QueryString( data[ (pos-1)/2] ) ){
                size_t temp=data[pos];
                data[pos]=data[ (pos-1)/2 ];
                data[ (pos-1)/2 ]=temp;
                pos=(pos-1)/2;
            }
            else break;
        }
    }

    ofstream ofs(argv[2]);
    ofs<<size;
    while(size>0){
        ofs<<endl<<parser.QueryString(data[0])<<" "<<data[0]+1;
        size_t pos=0;
        size_t temp;
        data[0]=data[--size];
        while( (2*pos+1)<size ){
            if( 2*pos+2==size ){
                if( parser.QueryString(data[pos])>parser.QueryString(data[pos*2+1]) ){
                    temp=data[pos];
                    data[pos]=data[pos*2+1];
                    data[pos*2+1]=temp;
                    pos=pos*2+1;
                }
                else break;
            }
            else{
                size_t smaller=pos*2+1;
                if( parser.QueryString(data[pos*2+2])<parser.QueryString(data[pos*2+1]) )
                    smaller=pos*2+2;
                if( parser.QueryString(data[pos])>parser.QueryString(data[smaller]) ){
                    temp=data[pos];
                    data[pos]=data[smaller];
                    data[smaller]=temp;
                    pos=smaller;
                }
                else break;
            }
        }
    }
}