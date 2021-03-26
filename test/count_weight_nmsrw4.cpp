#include <iostream>
#include <igraph.h>
#include <vector>
#include <algorithm>
#define GNUM 6
using namespace std;

int main(){
    vector<igraph_t> graph(GNUM);
    igraph_small(&graph[0],4,IGRAPH_UNDIRECTED,0,1,0,2,0,3,-1);
    igraph_small(&graph[1],4,IGRAPH_UNDIRECTED,0,1,1,2,2,3,-1);
    igraph_small(&graph[2],4,IGRAPH_UNDIRECTED,0,1,1,2,2,3,1,3,-1);
    igraph_small(&graph[3],4,IGRAPH_UNDIRECTED,0,1,1,2,2,3,0,3,-1);
    igraph_small(&graph[4],4,IGRAPH_UNDIRECTED,0,1,1,2,2,3,0,2,0,3,-1);
    igraph_small(&graph[5],4,IGRAPH_UNDIRECTED,0,1,1,2,2,3,0,2,0,3,1,3,-1);

    vector<int> per = {0,1,2,3};
    int cc[GNUM];
    for(int gid = 0;gid<GNUM;gid++){
        int count=0;
        do{
            int num=1;
            for(int i=1;i<4;i++){
                int k=0;
                for(int j=0;j<i;j++){
                    int eid;
                    igraph_get_eid(&graph[gid],&eid,per[i],per[j],IGRAPH_UNDIRECTED,0);
                    if(eid !=-1) k++; 
                }
                num*=k;
            }
            count+=num; 
        }while(next_permutation(per.begin(),per.end()));
        cc[gid]=count;
    }
    for(auto i:cc) cout<<i<<",";
    cout<<endl;
    return 0;
}