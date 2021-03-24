#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <queue>
#include <fstream>
using namespace std;

int main(){
    ifstream infile("catster.5g");
    vector<double> vc;
    double a,b,sum;
    while(infile>>a>>b){
        vc.push_back(b);
        sum+=b;
    }
    for(auto &i:vc) i=i/sum;
    ofstream outfile("catster.5c");
    outfile.precision(52);
    for(int i=0;i<vc.size();i++) outfile<<i<<"\t"<<vc[i]<<endl;
    return 0;
}