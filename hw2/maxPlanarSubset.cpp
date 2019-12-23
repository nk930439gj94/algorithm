#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
class MPS;

class MPS
{
public:
    MPS() { _N=0; }
    MPS(const string& inputfile){
        _N=0;
        read(inputfile);
    }
    ~MPS(){
        for(size_t i=0;i<_N;++i){
            delete [] _grid[i];
            delete [] _trace[i];
        }
        delete [] _grid;
        delete [] _trace;
        delete _chords;
    }

    void read(const string&);
    size_t compute() { return MIS(0,2*_N-1); }
    void write(const string&);

private:
    size_t _N;
    size_t* _chords;
    int** _grid;
    bool** _trace;

    size_t MIS(int,int);
    void trace(vector<size_t>&,int,int);
    int get(int i,int j,bool trace=false){
        if(j<=i) return 0;
        if(trace) return _trace[i][j-i-1];
        else return _grid[i][j-i-1];
    }
    void set(int value,int i,int j,bool trace=false){
        if(trace) _trace[i][j-i-1]=value;
        else _grid[i][j-i-1]=value;
    }
};

void MPS::read(const string& inputfile)
{
    ifstream file( inputfile );
    file>>_N;
    _N/=2;
    _chords=new size_t[2*_N];
    size_t n1, n2;
    for(size_t i=0;i<_N;++i){
        file>>n1;
        file>>n2;
        _chords[n1]=n2;
        _chords[n2]=n1;
    }
    
    _grid = new int*[2*_N-1];
    _trace = new bool*[2*_N-1];

    for(size_t i=0;i<2*_N-1;++i){
        _grid[i]=new int[2*_N-1-i];
        _trace[i]=new bool[2*_N-1-i]();
        for(size_t j=0;j<2*_N-1-i;++j){
            _grid[i][j]=-1;
        }
    }
}

size_t MPS::MIS(int i,int j)
{
    if(j<=i) return 0;
    int k=_chords[j];
    if( k>=i && k<j ){
        size_t n1=get(i,k-1);
        size_t n2=get(k+1,j-1);
        size_t n3=get(i,j-1);
        if(n1==-1) n1=MIS(i,k-1);
        if(n2==-1) n2=MIS(k+1,j-1);
        if(n3==-1) n3=MIS(i,j-1);
        if( ( n1+n2+1 ) >= n3 ){
            set(n1+n2+1,i,j);
            set(1,i,j,true);
            return n1+n2+1;
        }
        else{
            set(n3,i,j);
            return n3;
        }
    }
    else{
        size_t n=get(i,j-1);
        if(n==-1) n=MIS(i,j-1);
        set(n,i,j);
        return n;
    }
}

void MPS::trace(vector<size_t>& v,int i,int j)
{
    if(j<=i) return;
    int k=_chords[j];
    if( k>=i && k<j ){
        if( get(i,j,true)==1 ){
            v.push_back(k);
            trace(v,i,k-1);
            trace(v,k+1,j-1);
        }
        else
            trace(v,i,j-1);
    }
    else{
        trace(v,i,j-1);
    }
}

void MPS::write(const string& outputfile)
{
    ofstream file(outputfile);
    vector<size_t> v;
    trace(v,0,2*_N-1);
    sort(v.begin(),v.end());
    file<<v.size();
    for(size_t i=0;i<v.size();++i){
        file<<endl<<v[i]<<" "<<_chords[v[i]];
    }
}



int main(int argc, char** argv)
{
    MPS mps;
    mps.read( string(argv[1]) );
    mps.compute();
    mps.write( string(argv[2]) );
}