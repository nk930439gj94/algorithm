#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

int main(int argc, char** argv)
{
    ifstream file(argv[1]);
    size_t N, n1, n2;
    int temp=-1;
    vector< pair<size_t,size_t> > chords;
    file>>N;
    chords.reserve(N);
    for(size_t i=0;i<N;++i){
        file>>n1;
        file>>n2;
        if(n1>n2){
            cout<<"ERROR: wrong order (h)\n";
            cout<<"( "<<n1<<" , "<<n2<<" )\n";
            return -1;
        }
        if(int(n1)<temp){
            cout<<"ERROR: wrong order (v)\n";
            cout<<"( "<<n1<<" , "<<n2<<" )\n";
            cout<<temp<<endl;
            return -1;
        }
        temp=n1;
        chords.push_back( make_pair(n1,n2) );
    }

    for(size_t i=0;i<N-1;++i){
        for(size_t j=i+1;j<N;++j){
            if( chords[j].first < chords[i].second && chords[j].second > chords[i].second )
                cout<<"ERROR: bench\n";
                cout<<"( "<<chords[i].first<<" , "<<chords[i].second<<" )\n";
                cout<<"( "<<chords[j].first<<" , "<<chords[j].second<<" )\n";
                return -1;
        }
    }
}