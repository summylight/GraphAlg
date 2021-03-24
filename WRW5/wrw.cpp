#include <iostream>
#include <fstream>
#include <igraph.h>
#include <set>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
using namespace std;
bool get_edge_status(const igraph_t *ig, const int from, const int to);
int get_graphletid5(const igraph_t *ig);

int main()
{
    int repeat_all = 1000;
    int walklen = 50000;
    string str = "amazon.escape";
    string realcount_str = "amazon.5c";
    igraph_t G;
    char *filename = (char *)str.data();
    FILE *instream = fopen(filename, "r");
    igraph_read_graph_ncol(&G, instream, NULL, 0, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
    vector<vector<long double>> res;
    for (int repeat_time = 0; repeat_time < repeat_all; repeat_time++)
    {
        //随机游走至收敛
        RNG_BEGIN();
        int startnode = RNG_UNIF(0, igraph_vcount(&G));
        igraph_vector_t walknode;
        igraph_vector_init(&walknode, 0);
        igraph_random_walk(&G, &walknode, startnode, IGRAPH_ALL, 10000, IGRAPH_RANDOM_WALK_STUCK_ERROR);

        int len = igraph_vector_size(&walknode);
        if(len!=10000) cout<<"Can't walk into convege"<<endl;
        startnode = VECTOR(walknode)[len - 1];

        //得到收敛后的路径
        igraph_random_walk(&G, &walknode, startnode, IGRAPH_ALL, walklen, IGRAPH_RANDOM_WALK_STUCK_ERROR);

        igraph_vector_t degrees;
        igraph_vector_init(&degrees, walklen);
        igraph_vs_t vs;
        igraph_vs_vector(&vs, &walknode);
        igraph_degree(&G, &degrees, vs, IGRAPH_ALL, IGRAPH_NO_LOOPS);
        vector<int> weight = {24, 2, 4, 4, 2, 4, 12, 12, 2, 4, 8, 10, 14, 20, 8, 12, 36, 28, 48, 72, 120};
        set<int> s;
        vector<long double> cc(21);

        igraph_vector_t neis;
        igraph_vector_init(&neis, 0);
        for (int i = 4; i < walklen; i++)
        {
            // 找path 为5的子图
            s.clear();
            bool ContainDuplicate = 0;
            vector<int> vc;
            for (int k = 0; k < 5; k++)
            {
                vc.insert(vc.end(), VECTOR(walknode)[i - 4 + k]);
                if (!s.insert(VECTOR(walknode)[i - 4 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }

            if (!ContainDuplicate)
            {

                igraph_t subgraph;
                igraph_vector_t ivc;
                igraph_vector_init(&ivc, 0);
                for (int ii = 0; ii < 4; ++ii)
                {
                    for (int j = ii + 1; j < 5; ++j)
                    {
                        if (get_edge_status(&G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[i - 1] * VECTOR(degrees)[i - 2] * VECTOR(degrees)[i - 3];
                int gid = get_graphletid5(&subgraph);
                if(gid ==0 ||gid == 1 ||gid==2)
                    cout<<"here"<<endl;
                cc[get_graphletid5(&subgraph)] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }

            // 找path 为4的子图
            ContainDuplicate = 0;
            s.clear();
            vc.clear();
            for (int k = 0; k < 4; k++)
            {
                vc.insert(vc.end(), VECTOR(walknode)[i - 3 + k]);
                if (!s.insert(VECTOR(walknode)[i - 3 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }
            igraph_neighbors(&G, &neis, VECTOR(walknode)[i - 2], IGRAPH_ALL);
            int neissize = igraph_vector_size(&neis);
            int user = RNG_UNIF(0, neissize - 1);
            vc.insert(vc.end(), VECTOR(neis)[user]);
            if (!s.insert(vc[5]).second)
            {
                ContainDuplicate = 1;
            }

            if (!ContainDuplicate)
            {

                igraph_t subgraph;
                igraph_vector_t ivc;
                igraph_vector_init(&ivc, 0);
                for (int ii = 0; ii < 4; ++ii)
                {
                    for (int j = ii + 1; j < 5; ++j)
                    {
                        if (get_edge_status(&G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[i - 1] * VECTOR(degrees)[i - 2] * VECTOR(degrees)[i - 2];
                int id = get_graphletid5(&subgraph);
                if(id==2||id==1) cc[id] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }

            // 找path 为3的子图
            s.clear();
            ContainDuplicate = 0;
            vc.clear();
            for (int k = 0; k < 3; k++)
            {
                vc.insert(vc.end(), VECTOR(walknode)[i - 2 + k]);
                if (!s.insert(VECTOR(walknode)[i - 2 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }
            igraph_neighbors(&G, &neis, VECTOR(walknode)[i - 1], IGRAPH_ALL);
            neissize = igraph_vector_size(&neis);
            int user1 = RNG_UNIF(0, neissize - 1);
            int user2 = RNG_UNIF(0, neissize - 1);
            vc.insert(vc.end(), VECTOR(neis)[user1]);
            vc.insert(vc.end(), VECTOR(neis)[user2]);
            if (!s.insert(vc[4]).second)
            {
                ContainDuplicate = 1;
            }
            if (!s.insert(vc[5]).second)
            {
                ContainDuplicate = 1;
            }
            if (!ContainDuplicate)
            {
                igraph_t subgraph;

                igraph_vector_t ivc;
                igraph_vector_init(&ivc, 0);
                for (int ii = 0; ii < 4; ++ii)
                {
                    for (int j = ii + 1; j < 5; ++j)
                    {
                        if (get_edge_status(&G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[i - 1] * VECTOR(degrees)[i - 1] * VECTOR(degrees)[i - 1];
                int id = get_graphletid5(&subgraph);
                if(id==0) cc[id] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }
        }
        RNG_END();
        long double sum=0;
        for (int i = 0; i < 21; i++){
            cc[i] = (cc[i] / weight[i]);
            sum+=cc[i];
        }
        for (int i = 0; i < 21; i++){
            cc[i] /=sum;
        }
        res.push_back(cc);
    }


    ifstream cfile(realcount_str);
    vector<long double> realc;
    double a,b;
    while(cfile>>a>>b) realc.push_back(b);


    vector<long double> ans(21);
    vector<long double> nrmse(21);
    for(auto i:res){
        for(int j=0;j<i.size();j++){
            long double cha = (i[j] - realc[j]) / realc[j];
            nrmse[j]+=cha*cha;
            ans[j]+=i[j]/repeat_all;
        }
    }
    long double all = 0;
    for(int i=0;i<nrmse.size();i++){
        nrmse[i]=sqrt(nrmse[i]/res.size());
        cout<<i<<" nrsme: "<<nrmse[i]<<" mean :" <<abs(ans[i]-realc[i])/realc[i]<<endl;
    }
    return 0;
}

bool get_edge_status(const igraph_t *ig, const int from, const int to)
{
    igraph_integer_t eid;
    igraph_get_eid(ig, &eid, from, to, IGRAPH_UNDIRECTED, 0);
    if (eid == -1)
        return false;
    else
        return true;
}
int get_graphletid5(const igraph_t *ig)
{
    igraph_vector_t v;
    igraph_vector_init(&v, 0);
    igraph_degree(ig, &v, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
    static int degree[5];
    static vector<int> copy_degree(5);
    int id;
    for (int i = 0; i < 5; i++)
    {
        degree[i] = VECTOR(v)[i];
    }
    copy_degree[0] = degree[0];
    copy_degree[1] = degree[1];
    copy_degree[2] = degree[2];
    copy_degree[3] = degree[3];
    copy_degree[4] = degree[4];
    sort(degree, degree + 5);

    if (degree[0] == 1 && degree[1] == 1 && degree[2] == 2 && degree[3] == 2 && degree[4] == 2)
    {
        id = 8;
    }
    else if (degree[0] == 1 && degree[1] == 1 && degree[2] == 1 && degree[3] == 2 && degree[4] == 3)
    {
        id = 1;
    }
    else if (degree[0] == 1 && degree[1] == 1 && degree[2] == 1 && degree[3] == 1 && degree[4] == 4)
    {
        id = 0;
    }
    else if (degree[0] == 1 && degree[1] == 1 && degree[2] == 2 && degree[3] == 3 && degree[4] == 3)
    {
        id = 4;
    }

    else if (degree[0] == 1 && degree[1] == 2 && degree[2] == 2 && degree[3] == 2 && degree[4] == 3)
    {
        int index;
        for (int i = 0; i < copy_degree.size(); i++)
        {
            if (copy_degree[i] == 1)
            {
                index = i;
                break;
            }
        }
        for (int j = 0; j < copy_degree.size(); j++)
        {
            if (index == j)
                continue;
            igraph_integer_t eid;
            igraph_get_eid(ig, &eid, index, j, IGRAPH_UNDIRECTED, 0);
            if (eid != -1)
            {
                index = j;
                break;
            }
        }
        if (copy_degree[index] == 2)
            id = 9;
        else
            id = 3;
    }
    else if (degree[0] == 1 && degree[1] == 1 && degree[2] == 2 && degree[3] == 2 && degree[4] == 4)
    {
        id = 2;
    }

    if (degree[0] == 2 && degree[1] == 2 && degree[2] == 2 && degree[3] == 2 && degree[4] == 2)
    {
        id = 11;
    }
    else if (degree[0] == 1 && degree[1] == 2 && degree[2] == 2 && degree[3] == 3 && degree[4] == 4)
    {
        id = 5;
    }
    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 2 && degree[3] == 2 && degree[4] == 4)
    {
        id = 10;
    }
    else if (degree[0] == 1 && degree[1] == 2 && degree[2] == 3 && degree[3] == 3 && degree[4] == 3)
    {
        id = 14;
    }

    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 2 && degree[3] == 3 && degree[4] == 3)
    {
        int index;
        vector<int> _index;
        for (int i = 0; i < copy_degree.size(); i++)
        {
            if (copy_degree[i] == 3)
            {
                index = i;
                break;
            }
        }
        for (int j = 0; j < copy_degree.size(); j++)
        {
            if (index == j)
                continue;
            igraph_integer_t eid;
            igraph_get_eid(ig, &eid, index, j, IGRAPH_UNDIRECTED, 0);
            if (eid != -1)
            {
                _index.push_back(j);
                //break;
            }
        }
        if (copy_degree[_index[0]] == 2 && copy_degree[_index[1]] == 2 && copy_degree[_index[2]] == 2)
            id = 6;
        else
            id = 12;
    }
    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 2 && degree[3] == 4 && degree[4] == 4)
    {
        id = 7;
    }

    if (degree[0] == 1 && degree[1] == 3 && degree[2] == 3 && degree[3] == 3 && degree[4] == 4)
    {
        id = 15;
    }
    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 3 && degree[3] == 3 && degree[4] == 4)
    {
        id = 13;
    }
    else if (degree[0] == 2 && degree[1] == 3 && degree[2] == 3 && degree[3] == 3 && degree[4] == 3)
    {
        id = 17;
    }
    else if (degree[0] == 2 && degree[1] == 3 && degree[2] == 3 && degree[3] == 4 && degree[4] == 4)
    {
        id = 16;
    }

    else if (degree[0] == 3 && degree[1] == 3 && degree[2] == 3 && degree[3] == 3 && degree[4] == 4)
    {
        id = 18;
    }
    else if (degree[0] == 3 && degree[1] == 3 && degree[2] == 4 && degree[3] == 4 && degree[4] == 4)
    {
        id = 19;
    }
    else if (degree[0] == 4 && degree[1] == 4 && degree[2] == 4 && degree[3] == 4 && degree[4] == 4)
    {
        id = 20;
    }
    return id;
}