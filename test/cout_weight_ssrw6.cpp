#include <igraph.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/graph.h"
using namespace std;

int main(){
    vector<igraph_t> graphlet6;
    init_igraph6(graphlet6);

    vector<int> a= {0,1,2,3,4,5};
    vector<int> weight(112);
    for(int i=0;i<112;i++){
        do{
            int w = 1;
            if(!get_edge_status(graphlet6[i],a[0],a[1])) continue;
            for(int j=2;j<6;j++){
                int c = 0;
                for(int k=1;k<j;k++){
                    if(get_edge_status(graphlet6[i],a[j],a[k])) c++;
                }
                if(c==0) w=0;
                else w*=c;
            }
            weight[i]+=w;
        }while(next_permutation(a.begin(),a.end()));
    }
    for(auto i:weight) cout<<i<<endl;
    return 0;
}


