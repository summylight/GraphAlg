#include <vector>
#include <list>

#include <iostream>

using namespace std;

int main(){
    vector<int> a;
    list<vector<int> > li;

    for(int i=0;i<3;i++){
        a.push_back(i);
        li.push_back(a);
    }
    for(auto i:li){
        for(auto k:i) cout<<k<<"\t ";
        cout<<endl;
    }
    return 0;
}