#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int args, char** argv)
{
    ifstream f;
    f.open(argv[1]);
    int size, n1, n2;
    vector<int> left, right;
    f >> size;
    for(int i = 0; i < size; i++)
    {
        f >> n1;
        f >> n2;
        left.push_back(n1);
        right.push_back(n2);
    }
    int prev = -1;
    bool flag = 0;
    for(int i = 0; i < size; i++)
    {
        
        if (left[i] > right[i] || left[i] < prev) {
            cout << "ERROR!" << endl;
            cout << "l_i = " << left[i] << ", r_i = " << right[i] << endl;
            cout << "prev = " << prev << endl;
            break; 
        }
        
        for(int j = i+1; j < size; j++)
        {
            if((left[i] < left[j] && right[i] > right[j]) || right[i] < left[j]){
                continue;
            }
            else{
                cout << "ERROR!" << endl;
                cout << "l_i = " << left[i] << ", r_i = " << right[i] << endl;
                cout << "l_j = " << left[j] << ", r_j = " << right[j] << endl;
                flag = 1;
                break;
            }
        }
        
        if(flag) break;
        prev = left[i];        
        
    }
    
}