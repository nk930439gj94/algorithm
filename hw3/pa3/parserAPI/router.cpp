#include "parser.h"
#include <climits>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;


class Node
{
friend class Router;
public:
    Node() {}
private:
    Node* _Predecessor;
    double _d;
    unsigned _i,_j;
    bool _isblack;

    void set( unsigned i, unsigned j ) { _i=i; _j=j; }
};



class Router
{
    typedef pair< unsigned, pair< Node*, Node* > > net;
public:
    Router() { _Grid=0; }
    Router( char *f ) { read(f); }
    ~Router();
    void read( char* );
    void routing_apsp();
    void write( char* );

private:
    unsigned _m,_n;
    Node **_Grid;
    unsigned _Capacity;
    vector<Node*> **_Edges;     // adjacent list
    vector<double> **_Edge_weights;
    vector<unsigned> **_Edge_demands;
    vector< net > _Nets;    // net: ( id, ( s, t ) )
    vector< pair< unsigned, vector< Node* > > > _Routingresults;    // ( id, [path] )

    struct nodecompare
    {
        bool operator()( const Node &l, const Node &r ) { return ( l._d > r._d ); }
    };
    priority_queue< Node, vector<Node>, nodecompare > _pq;

    void initialize();
    void routing_sssp( unsigned );
    void backtrace( Node*, unsigned );
    void increaseweight( Node* );


    // for net ordering
    // 1st: netlength
    struct netlengthcompare
    {
        bool operator() ( const net &l, const net &r ){
            unsigned d1 = manhattandistance( l.second.first, l.second.second );
            unsigned d2 = manhattandistance( r.second.first, r.second.second );
            return d1 < d2;
        }
        unsigned manhattandistance( Node *a, Node *b ){
            unsigned x;
            if( a->_i > b->_i ) x = ( a->_i - b->_i );
            else x = ( b->_i - a->_i );
            if( a->_j > b->_j ) x += ( a->_j - b->_j );
            else x += ( b->_j - a->_j );
            return x;
        }
    };
    
};





Router::~Router()
{
    if( _Grid!=0 ){
        for( int i=0; i<_n; ++i ){
            delete [] _Grid[i];
            delete [] _Edges[i];
            delete [] _Edge_weights[i];
            delete [] _Edge_demands[i];
        }
        delete [] _Grid;
        delete [] _Edges;
        delete [] _Edge_weights;
        delete [] _Edge_demands;
    }
}


void Router::read( char *f )
{
    AlgParser parser;
    parser.read(f);
    _m = parser.gNumHTiles();
    _n = parser.gNumVTiles();
    _Capacity = parser.gCapacity();

    // construct grid
    _Grid = new Node*[_n];
    for( int i=0; i<_n; ++i ) _Grid[i] = new Node[_m]();
    for( int i=0; i<_m; ++i ){
        for( int j=0; j<_n; ++j ) _Grid[i][j].set(i,j);
    }

    // construc edges
    _Edges = new vector<Node*>*[_n];
    _Edge_weights = new vector<double>*[_n];
    _Edge_demands = new vector<unsigned>*[_n];
    for( int i=0; i<_n; ++i ){
        _Edges[i] = new vector<Node*>[_m];
        _Edge_weights[i] = new vector<double>[_m];
        _Edge_demands[i] = new vector<unsigned>[_m];
    }
    for( int i=0; i<_m; ++i ){
        for( int j=0; j<_n; ++j ){
            if( i>0 ){
                _Edges[i][j].push_back( &_Grid[i-1][j] );
                _Edge_weights[i][j].push_back(1);
                _Edge_demands[i][j].push_back(0);
            }
            if( i<_m-1 ){
                _Edges[i][j].push_back( &_Grid[i+1][j] );
                _Edge_weights[i][j].push_back(1);
                _Edge_demands[i][j].push_back(0);
            }
            if( j>0 ){
                _Edges[i][j].push_back( &_Grid[i][j-1] );
                _Edge_weights[i][j].push_back(1);
                _Edge_demands[i][j].push_back(0);
            }
            if( j<_n-1 ){
                _Edges[i][j].push_back( &_Grid[i][j+1] );
                _Edge_weights[i][j].push_back(1);
                _Edge_demands[i][j].push_back(0);
            }
        }
    }

    // construct nets
    unsigned numNets = parser.gNumNets();
    for( int i=0; i<numNets; ++i ){
        Node *s = &_Grid[ parser.gNetStart(i).first ][ parser.gNetStart(i).second ];
        Node *t = &_Grid[ parser.gNetEnd(i).first ][ parser.gNetEnd(i).second ];
        _Nets.push_back( make_pair( i, make_pair(s,t) ) );
    }
}


void Router::initialize()
{
    for( int i=0; i<_m; ++i ){
        for(int j=0;j<_n;++j){
            _Grid[i][j]._d = UINT_MAX;
            _Grid[i][j]._isblack = false;
        }
    }
    _pq = priority_queue< Node, vector<Node>, nodecompare >();
}


void Router::routing_apsp()
{
    sort( _Nets.begin(), _Nets.end(), netlengthcompare() );
    for( int i=0; i<_Nets.size(); ++i ) { routing_sssp(i); }
}


void Router::routing_sssp( unsigned n )    // routing _Nets[n]
{
    initialize();
    unsigned netid = _Nets[n].first;
    Node *t = _Nets[n].second.second;
    Node *min = _Nets[n].second.first;
    min->_d=0;
    min->_Predecessor=0;
    min->_isblack = true;
    while( min!=t ){
        for( int i=0; i<_Edges[min->_i][min->_j].size(); ++i ){
            Node* adjacent = _Edges[min->_i][min->_j][i];
            double weight = _Edge_weights[min->_i][min->_j][i];
            if( adjacent->_isblack ) continue;
            if( (min->_d + weight) < adjacent->_d ){
                adjacent->_d = (min->_d + weight);
                adjacent->_Predecessor = min;
                _pq.push(*adjacent);
            }
        }
        while( min->_isblack ){
            min = &_Grid[_pq.top()._i][_pq.top()._j];
            _pq.pop();
        }
        min->_isblack = true;
    }
    increaseweight( t );
    backtrace( t, netid );
}


void Router::backtrace( Node *t, unsigned netid )
{
    Node* n = t;
    vector<Node*> v;
    while( n!=0 ){
        v.push_back(n);
        n = n->_Predecessor;
    }
    _Routingresults.push_back( make_pair( netid, v ) );
}


void Router::increaseweight( Node *t )
{
    Node *n = t, *pre = n->_Predecessor;
    while( pre != 0 ){
        int i=0,j=0;
        while( _Edges[n->_i][n->_j][i] != pre ) ++i;
        while( _Edges[pre->_i][pre->_j][j] != n ) ++j;
        ++_Edge_demands[n->_i][n->_j][i];
        ++_Edge_demands[pre->_i][pre->_j][j];
        
        double newweight = pow( 2, double(_Edge_demands[n->_i][n->_j][i])/_Capacity );
        if( _Edge_demands[n->_i][n->_j][i] >= _Capacity ) newweight = 3;
        
        _Edge_weights[n->_i][n->_j][i] = newweight;
        _Edge_weights[pre->_i][pre->_j][j] = newweight;
        n = pre;
        pre = n->_Predecessor;
    }
}


void Router::write( char* f )
{
    ofstream file(f);
    for( int i=0; i<_Routingresults.size(); ++i ){
        file<<_Routingresults[i].first<<" "<<_Routingresults[i].second.size()-1;
        for( int j=_Routingresults[i].second.size()-1; j>0; --j ){
            file<<endl;
            file<<_Routingresults[i].second[j]->_i<<" "<<_Routingresults[i].second[j]->_j<<" ";
            file<<_Routingresults[i].second[j-1]->_i<<" "<<_Routingresults[i].second[j-1]->_j;
        }
        if( i != _Routingresults.size()-1 ) file<<endl;
    }
}




int main(int argc,char **argv)
{
    Router r( argv[1] );
    r.routing_apsp();
    r.write( argv[2] );
}