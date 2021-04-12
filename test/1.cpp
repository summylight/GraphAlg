#include <iostream>
#include <igraph.h>
#include <fstream>
#include <vector>
#include <set>
#include "../include/graph.h"
using namespace std;

int main()
{
    vector<igraph_t> graph6;
    init_igraph6(graph6);

    ifstream infile("/home/guang/GraphAlg/test/ca.6g");
    ofstream outfile("/home/guang/GraphAlg/test/ca.6c");
    if(!outfile) cout<<"open fail"<<endl;
    int a,b;
    vector<int> res(112);
    long double sum = 0;
    while (infile>>a>>b)
    {
        res[a]=b;
        sum+=b;
    }
    for(int i=0;i<112;i++){
        outfile<<i<<"\t"<<res[i]/sum<<endl;
    }
    return 0;
}