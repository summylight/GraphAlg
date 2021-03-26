#include <iostream>
#include <igraph.h>
#include <fstream>
#include <vector>
using namespace std;

int main(){
    
    vector<int> d(10);
    for(int i=0;i<1000;i++){
        RNG_BEGIN();
        cout<<RNG_INTEGER(0,8)<<endl;
        RNG_END();
    }
    for(auto i:d) cout<<i<<endl;

    return 0;
}