#include <igraph.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <set>
#include <random>
#include <unordered_map>
#define MOTIF5_NUM 21 //5-subgraph count
//Set for once random walk jump distance
using namespace std;

int getRandUniform(int range){
     return RNG_INTEGER(0,range-1);
//    uniform_int_distribution<> dist(0,range-1);
//    return dist(rng);
}
int RandStart(igraph_t *graph);
int RandWalk(igraph_t *graph, int steps, int startNode);
int START_JUMP = 0;

/**
 * \function print_vector
 * Print elements in an igraph vector  
 * 
 * \param vector pointer
 * \param outfile FILE pointer (using stdout can print to the screen)
 * 
 * */
void print_vector(igraph_vector_t *v, FILE *f)
{
    int i;
    for (i = 0; i < igraph_vector_size(v); i++)
    {
        fprintf(f, " %i", (int)VECTOR(*v)[i]);
    }
    fprintf(f, "\n");
}
/**
 * @brief Get the edge status object
 * 
 * @param graph igraph_t object
 * @param from  from vectex id
 * @param to to vectex id
 * @return true there exists an edge between from and to 
 * @return false no edge
 */
bool get_edge_status(igraph_t &graph, int from, int to)
{
    igraph_integer_t eid;
    igraph_get_eid(&graph, &eid, from, to, IGRAPH_UNDIRECTED, 0);
    if (eid == -1)
        return false;
    else
        return true;
}
/**
 * @brief Get the graphletid 
 * 
 * @param graph graph of 5 vectex
 * @return graphletid 
 */
int get_graphletid(igraph_t &graph);

/**
 * \function RandStart
 * Create a startnode that  random walk could start from while obeying Stationary process 
 * 
 * \param graph the input graph
 * \return a startnode id
 * */
int RandStart(igraph_t *graph)
{
    long long v_size = igraph_vcount(graph);
    long long tmp = getRandUniform(v_size);
    return RandWalk(graph, 10000, tmp);
}
/**
 * \function RandWalk
 * Perform a randwalk on one given node
 * 
 * \param graph the input graph
 * \param steps the random walk will run
 * \param startNode random walk startNode
 * */
int RandWalk(igraph_t *graph, int steps, int startNode)
{
    igraph_vector_t v;
    igraph_vector_init(&v, 0);
    int nextNode = startNode;
    for (int i = 0; i < steps; ++i)
    {
        igraph_neighbors(graph, &v, nextNode, IGRAPH_ALL);
        //        print_vector(&v, stdout);
        int pos = getRandUniform(igraph_vector_size(&v));
        //        printf("step: %d size : %ld choose :%ld \t",i,igraph_vector_size(&v),next);
        nextNode = VECTOR(v)[pos];
        //        cout <<nextNode <<"randwalk" <<endl;
    }
    //    cout <<nextNode <<"END" <<endl;
    return nextNode;
}
void count_nmrseg(string graph_name, vector<vector<long double>> &res, string outfile_name)
{
    string file_5g_dt = "/home/guang/graph/graphset/" + graph_name + ".5g";

    ifstream instream(file_5g_dt);

    vector<long double> real(MOTIF5_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nmrse(MOTIF5_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            long double cha = (res[i][j] - real[j]) / real[j];
            nmrse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF5_NUM; j++)
    {
        nmrse[j] = nmrse[j] / res.size();
        nmrse[j] = sqrt(nmrse[j]);
    }

    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out.precision(15);
    for (int i = 0; i < MOTIF5_NUM; ++i)
    {
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}

void count_nmrsec(string graph_name, vector<vector<long double>> &res, string outfile_name)
{
    string file_5g_dt = "/home/guang/graph/graphset/" + graph_name + ".5c";

    ifstream instream(file_5g_dt);

    vector<long double> real(MOTIF5_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nmrse(MOTIF5_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        double sum=0;
        for (int j = 0; j < MOTIF5_NUM; j++){
            sum+=res[i][j];
        }
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            long double cha = (res[i][j]/sum - real[j]) / real[j];
            nmrse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF5_NUM; j++)
    {
        nmrse[j] = nmrse[j] / res.size();
        nmrse[j] = sqrt(nmrse[j]);
    }

    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out.precision(52);
    for (int i = 0; i < MOTIF5_NUM; ++i)
    {
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}


/**
 * \function main
 * 
 * Based on SSRW(Yang.C) algrithem
 * 
 * \param argv[1] the input graphfile ,NOTICE that file must like vertice one ,vertice two \
 * \input graph should be an undirected graph , connected and contain no ring 
 * \param argv[2] the times you want to run the randwalk ,the more you run, the rusult may be more accurate
 * */

int main(int argc, char *argv[])
{
    random_device rd;

    double dur = 0;
    struct timeval start, end, realstart, realend;
    if (argc < 5)
    {
        printf("The pramament of the procedure is like ./SSRW5 amazon/catster/dblp/enron/gnutella/live/pokec run_time_single_turn jump_len(1) repeat_time\n");
        exit(1);
    }
    vector<vector<long double>> res;
    string graph_name = argv[1];
    string file_dt = "/home/guang/graph/graphset/" + graph_name + ".escape";
    char *filename = (char *)file_dt.c_str();
    int walklen = atol(argv[2]);
    int jump_len = atol(argv[3]);
    int repeat_time = atoi(argv[4]);
    FILE *instream = fopen(filename, "r");
    if (!instream)
    {
        string msg = "Open file fail!:";
        msg.append(filename);
        throw runtime_error(msg);
    }
    cout << "Now read " << file_dt << endl;

    string s = graph_name + "_" + to_string(walklen) + "_" + to_string(jump_len) + "_" + to_string(repeat_time);

    igraph_t G;
    gettimeofday(&start, NULL);
    gettimeofday(&realstart, NULL);
    igraph_read_graph_ncol(&G, instream, NULL, 0, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
    gettimeofday(&end, NULL);
    dur = dur + (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
    cout << "read time : " << dur << endl;
    dur = 0;

    igraph_t graph[MOTIF5_NUM];

    // igraph_small(&graph4[0], 4, IGRAPH_UNDIRECTED, 0, 3, 1, 3, 2, 3, -1);
    // igraph_small(&graph4[1], 4, IGRAPH_UNDIRECTED, 0, 3, 0, 2, 1, 3, -1);
    // igraph_small(&graph4[2], 4, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 1, 3, 2, 3, -1);
    // igraph_small(&graph4[3], 4, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 1, 2, 1, 3, -1);
    // igraph_small(&graph4[4], 4, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3, -1);
    // igraph_small(&graph4[5], 4, IGRAPH_UNDIRECTED, 0, 1, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3, -1);

    igraph_small(&graph[0], 5, IGRAPH_UNDIRECTED, 0, 4, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[1], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 4, 2, 4, -1);
    igraph_small(&graph[2], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[3], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 4, 2, 4, -1);
    igraph_small(&graph[4], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 2, 4, 3, 4, -1);
    igraph_small(&graph[5], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[6], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 4, 2, 3, 2, 4, -1);
    igraph_small(&graph[7], 5, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4, -1);
    igraph_small(&graph[8], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 1, 3, 1, 4, -1);
    igraph_small(&graph[9], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 1, 3, 1, 4, 2, 4, -1);
    igraph_small(&graph[10], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 1, 3, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[11], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 1, 3, 1, 4, 2, 4, -1);
    igraph_small(&graph[12], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 3, 1, 4, 2, 4, -1);
    igraph_small(&graph[13], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 3, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[14], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 4, 2, 3, 2, 4, -1);
    igraph_small(&graph[15], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 4, 2, 3, 2, 4, 3, 4, -1);
    igraph_small(&graph[16], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4, -1);
    igraph_small(&graph[17], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 2, 1, 3, 1, 4, 2, 4, -1);
    igraph_small(&graph[18], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 2, 1, 3, 1, 4, 2, 4, 3, 4, -1);
    igraph_small(&graph[19], 5, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4, -1);
    igraph_small(&graph[20], 5, IGRAPH_UNDIRECTED, 0, 1, 0, 2, 0, 3, 0, 4, 1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4, -1);


    //    int W_constant[] = {48, 28, 104, 88, 72, 220, 264, 528, 16, 60, 224, 80, 224, 488, 176, 396, 944, 504, 992, 1728, 2880};
    int W_constant[]={0,0,0,2,0,2,12,12,0,0,0,0,4,8,2,6,24,16,32,60,120};

    // int a[6][21];
    // for (int i = 0; i < 6; i++)
    //     for (int j = 0; j < 21; j++)
    //     {
    //         igraph_bool_t iso;
    //         igraph_subisomorphic(&graph[j], &graph4[i], &iso);
    //         if (iso)
    //             a[i][j] = 1;
    //         else
    //             a[i][j] = 0;
    //     }

    // unordered_map<string, int> series_map_7;
    // unordered_map<string, int> series_map_10;


        
    for (int iii = 0; iii < repeat_time; ++iii)
    {
        igraph_rng_seed(igraph_rng_default(), rd());
        int startid = RandStart(&G);
//        cout<<"Turn "<<iii<<" start from "<<startid<<endl;
        vector<long double> count(MOTIF5_NUM); //count times all subgraph may appear
        int run_times = 0;

        gettimeofday(&start, NULL);
        int walk = 0;
        long count_times = 0;
        int flag = 0;
        long sample_times = 0;
        igraph_vector_t walknodes;
        igraph_vector_init(&walknodes,0);
        igraph_random_walk(&G,&walknodes,startid,IGRAPH_ALL,walklen,IGRAPH_RANDOM_WALK_STUCK_RETURN);

        igraph_vs_t vs;
        igraph_vs_vector(&vs, &walknodes);
        igraph_vector_t degrees;
        igraph_vector_init(&degrees,0);
        igraph_degree(&G, &degrees, vs, IGRAPH_ALL, IGRAPH_NO_LOOPS);
        igraph_vs_destroy(&vs);
        igraph_vector_t nei1,nei2,intersection;
        
        for (int i=2;i<walklen-1;i++)
        {
            igraph_vector_init(&nei1,0);
            igraph_neighbors(&G,&nei1,VECTOR(walknodes)[i-2],IGRAPH_ALL);


            igraph_vector_init(&nei2,0);
            igraph_neighbors(&G,&nei2,VECTOR(walknodes)[i],IGRAPH_ALL);

            igraph_vector_init(&intersection,0);
            igraph_vector_intersect_sorted(&nei1,&nei2,&intersection);

            int node5_user = RNG_UNIF(0,igraph_vector_size(&intersection)-1);
            int node5 = VECTOR(intersection)[node5_user];

            vector<int> node;
            for(int k=i-2;k<i+2;k++){
                node.push_back(VECTOR(walknodes)[k]);
            }
            node.push_back(node5);
            set<int> it(node.begin(), node.end());
            if (it.size() == 5)
            {
                long double dg_prod = igraph_vector_size(&intersection)*VECTOR(degrees)[i-1]*VECTOR(degrees)[i];
                igraph_t subgraph;
                igraph_vector_t vc;
                igraph_vector_init(&vc, 0);
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = i + 1; j < 5; ++j)
                    {
                        if (get_edge_status(G,node[i],node[j]))
                        {
                            igraph_vector_push_back(&vc, i);
                            igraph_vector_push_back(&vc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &vc, 0, IGRAPH_UNDIRECTED);
                int gid = get_graphletid(subgraph);
                count[gid] += dg_prod;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&vc);
            }
            startid = RandWalk(&G, jump_len, node[0]);
        }

        igraph_vector_destroy(&walknodes);
        igraph_vector_destroy(&degrees);



        gettimeofday(&end, NULL);
        dur = dur + (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;

        vector<long double> ans(MOTIF5_NUM);
        for (int i = 0; i < MOTIF5_NUM; ++i)
            ans[i] = ((count[i] / W_constant[i]) / (walklen-3) )* igraph_ecount(&G) * 2;

        ofstream out(s + ".ssrw5", std::ios_base::app);
        out.precision(15);
        for (int i = 0; i < MOTIF5_NUM; ++i)
        {
            out << i << " " << ans[i] << endl;
        }
        res.push_back(ans);
        out << dur << endl;
        out.close();
        //            cout<<sample_times<<endl;
    }
    printf("per sample Use Time:%f\n", dur / repeat_time); //count time
    cout << "SSRW and NMSRE is writing to " << s << ".ssrw5 and " << s << ".nmrse" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%f\n", dur); //count time
    string nmrse_file_name = graph_name + "_" + to_string(walklen) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + ".nrmse";
    count_nmrseg(graph_name, res, nmrse_file_name);

    ofstream graphlet0_deg4_file("graphlet0_deg4_node.txt");
    //for(auto i:graphlet0_deg4_node) graphlet0_deg4_file<<i<<"\n";
    graphlet0_deg4_file.close();
    return 0;
}

int get_graphletid(igraph_t &graph)
{
    igraph_vector_t v;
    igraph_vector_init(&v, 0);
    igraph_degree(&graph, &v, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
    int degree[5];
    vector<int> copy_degree(5);
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
            if (get_edge_status(graph, index, j))
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
            if (get_edge_status(graph, index, j))
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

