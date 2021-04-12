#include "../include/graph.h"
using namespace std;
int main(int argc, char *argv[])
{
    random_device rd;

    long double dur = 0;
    struct timeval start, end, realstart, realend;
    if (argc < 5)
    {
        printf("The pramament of the procedure is like ./ssrw6 ca/gnutella run_time_single_turn jump_len(1) repeat_time\n");
        exit(1);
    }
    vector<vector<vector<long double>>> res; //记录1000单位的数据
    vector<long double> count_time;
    string graph_name = argv[1];
    string file_dt = "/home/guang/graph/graphset/" + graph_name + ".escape";
    char *filename = (char *)file_dt.c_str();
    int given_time = atol(argv[2]);
    int jump_len = atol(argv[3]);
    int repeat_time = atoi(argv[4]);

    for (int i = 0; i < given_time / 1000; i++)
    {
        res.push_back(vector<vector<long double>>());
        count_time.push_back(0);
    }

    FILE *instream = fopen(filename, "r");
    if (!instream)
    {
        string msg = "Open file fail!:";
        msg.append(filename);
        throw runtime_error(msg);
    }
    cout << "Now read " << file_dt << endl;

    string s = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time);

    igraph_t G;
    gettimeofday(&start, NULL);
    gettimeofday(&realstart, NULL);
    igraph_read_graph_ncol(&G, instream, NULL, 0, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
    gettimeofday(&end, NULL);
    dur = dur + (end.tv_sec - start.tv_sec) + (long double)(end.tv_usec - start.tv_usec) / 1000000.0;
    cout << "read time : " << dur << endl;
    dur = 0;

    int W_constant[] = {120,42,204,10,32,104,84,340,40,216,188,600,576,1344,12,66,336,58,36,36,186,134,552,2,28,14,24,52,160,104,132,548,188,528,120,412,296,358,420,1024,356,264,892,912,1984,144,320,872,12,120,88,80,68,112,308,230,340,904,352,232,726,704,1656,610,580,1364,756,792,1640,3012,792,1452,1504,2892,5112,52,80,292,156,840,256,672,440,658,608,1520,1440,2928,280,204,596,1500,612,1354,1356,2684,4660,1274,1272,2596,978,2016,4428,7464,2548,2352,2448,4452,4368,7328,11520,17280};
    vector<vector<bool>> a = init_6graphletid();
    vector<igraph_t> graph6;
    init_igraph6(graph6);
    for (int iii = 0; iii < repeat_time; ++iii)
    {
        igraph_rng_seed(igraph_rng_default(), rd());
        int startid = RandStart(&G);
        long double count[MOTIF6_NUM]; //count times all subgraph may appear
        int run_times = 0;

        for (int i = 0; i < MOTIF6_NUM; ++i)
            count[i] = 0;
        gettimeofday(&start, NULL);
        vector<int> node(6), user(6);
        vector<long long> degree(5);
        igraph_vector_t *nodeneigh[5];
        igraph_lazy_adjlist_t adj;
        IGRAPH_CHECK(igraph_lazy_adjlist_init(&G, &adj, IGRAPH_ALL, IGRAPH_DONT_SIMPLIFY));
        IGRAPH_FINALLY(igraph_lazy_adjlist_destroy, &adj);
        dur = 0;
        for (; run_times < given_time; ++run_times)
        {
           node[0] = startid;
            nodeneigh[0] = igraph_lazy_adjlist_get(&adj, node[0]);
            degree[0] = igraph_vector_size(nodeneigh[0]);
            //           print_vector(&nodeneigh[0], stdout);
            user[1] = get_random(degree[0]);
            node[1] = VECTOR(*nodeneigh[0])[user[1]];

            nodeneigh[1] = igraph_lazy_adjlist_get(&adj, node[1]);
            degree[1] = igraph_vector_size(nodeneigh[1]);
            user[2] = get_random(degree[1]);
            node[2] = VECTOR(*nodeneigh[1])[user[2]];

            nodeneigh[2] = igraph_lazy_adjlist_get(&adj, node[2]);
            degree[2] = igraph_vector_size(nodeneigh[2]);
            user[3] = get_random(degree[1] + degree[2]);
            if (user[3] < degree[1])
                node[3] = VECTOR(*nodeneigh[1])[user[3]];
            else
                node[3] = VECTOR(*nodeneigh[2])[user[3] - degree[1]];

            nodeneigh[3] = igraph_lazy_adjlist_get(&adj, node[3]);
            degree[3] = igraph_vector_size(nodeneigh[3]);
            user[4] = get_random(degree[1] + degree[2] + degree[3]);
            if (user[4] < degree[1])
                node[4] = VECTOR(*nodeneigh[1])[user[4]];
            else if (user[4] < degree[1] + degree[2])
                node[4] = VECTOR(*nodeneigh[2])[user[4] - degree[1]];
            else
                node[4] = VECTOR(*nodeneigh[3])[user[4] - degree[2] - degree[1]];

            nodeneigh[4] = igraph_lazy_adjlist_get(&adj, node[4]);
            degree[4] = igraph_vector_size(nodeneigh[4]);
            user[5] = get_random(degree[1] + degree[2] + degree[4] + degree[3]);
            if (user[5] < degree[1])
                node[5] = VECTOR(*nodeneigh[1])[user[5]];
            else if (user[5] < degree[1] + degree[2])
                node[5] = VECTOR(*nodeneigh[2])[user[5] - degree[1]];
            else if (user[5] < degree[1] + degree[3] + degree[2])
                node[5] = VECTOR(*nodeneigh[3])[user[5] - degree[2] - degree[1]];
            else
                node[5] = VECTOR(*nodeneigh[4])[user[5] - degree[3] - degree[1] - degree[2]];


            long double dg_prod = (long double)(degree[3] +degree[1] + degree[2] + degree[4]) * (degree[3] + degree[1] + degree[2]) * ( degree[1]) * ( degree[1] + degree[2]);
            set<int> it(node.begin(), node.end());
            if (it.size() == 6)
            {
                igraph_t subgraph, subgraph5;
                igraph_vector_t vc;
                igraph_vector_init(&vc, 0);
                for (int i = 0; i < 4; ++i)
                {
                    //                cout << nodes [i] <<" ";
                    //                cout << igraph_vector_size(&neighs[i]) << " : ";
                    //               print_vector(&neighs[i],stdout);
                    for (int j = i + 1; j < 5; ++j)
                    {
                        if (igraph_vector_binsearch2(nodeneigh[i], node[j]))
                        {
                            igraph_vector_push_back(&vc, i);
                            igraph_vector_push_back(&vc, j);
                            //                    cout <<"find !!"<<nodes[i] <<' '<<nodes[j]<<endl;
                        }
                    }
                }
                igraph_create(&subgraph5, &vc, 0, IGRAPH_UNDIRECTED);
                igraph_bool_t bo;
                int whoisgraph5 = get_5graphletid(subgraph5);
                if (whoisgraph5 == -1)
                {
                    cout << "can't find graph5!!" << endl;
                    cout << igraph_vcount(&subgraph5) << endl;
                }
                for (int i = 0; i < 5; ++i)
                {
                    //                        cout << nodes [i] <<" ";
                    //                        cout << igraph_vector_size(&neighs[i]) << " : ";
                    //                       print_vector(&neighs[i],stdout);
                    if (igraph_vector_binsearch2(nodeneigh[i], node[5]))
                    {
                        igraph_vector_push_back(&vc, i);
                        igraph_vector_push_back(&vc, 5);
                        //                            cout <<"find !!"<<nodes[i] <<' '<<nodes[j]<<endl;
                    }
                }
                igraph_create(&subgraph, &vc, 0, IGRAPH_UNDIRECTED);
                igraph_bool_t ios = 0;
                for (int i = 0; i < 112; ++i)
                {
                    if (igraph_ecount(&subgraph) != igraph_ecount(&graph6[i]))
                        continue;
                    if (!a[i][whoisgraph5])
                        continue;
                    igraph_isomorphic(&subgraph, &graph6[i], &ios);
                    //                            cout << "iso result : " << ios <<endl;
                    if (ios)
                    {
                        count[i] = count[i] + dg_prod; //count isomorphic subgraph
                        break;
                    }
                }
                if (!ios)
                { //test if find isomorphic fail
                    cout << "FIND SUBGRAPH ISO FAIL!!" << endl;
                    cout << "Now print the graph edge:" << endl;
                    igraph_integer_t from, to;
                    for (int k = 0; k < igraph_ecount(&subgraph); k++)
                    {
                        igraph_edge(&subgraph, k, &from, &to);
                        printf(" Num %d edge : from %d to %d\n", k, from, to);
                    }
                    exit(1);
                }
                igraph_destroy(&subgraph);
            }
            startid = RandWalk(&G, jump_len, node[0]);
            if ((run_times + 1) % 1000 == 0)
            {
                gettimeofday(&end, NULL);
                dur = (end.tv_sec - start.tv_sec) + (long double)(end.tv_usec - start.tv_usec) / 1000000.0;

                vector<long double> ans(MOTIF6_NUM);
                for (int i = 0; i < MOTIF6_NUM; ++i){
                    if(count[i]!=0) ans[i] = ((count[i]) * igraph_ecount(&G) * 2 / W_constant[i]) / (run_times + 1);
                }


                res[run_times / 1000].push_back(ans);
                count_time[run_times / 1000] += dur;
            }
        }

        igraph_lazy_adjlist_destroy(&adj);
        IGRAPH_FINALLY_CLEAN(1);
    }

    //时间统计
    string time_file_name = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "timessrw6.txt";
    ofstream out(time_file_name);

    printf("Sample Use Time: %Lf s per sample Use Time:%Lf s\n", dur, (dur / repeat_time));
    cout << "SSRW and NMSRE is writing to " << s << "gssrw5.txt" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (long double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%Lf\n", dur); //count time
    string nrmse_file_nameg = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "gssrw6.txt";
    for (int i = 0; i < given_time / 1000; i++)
    {
        string str_times = to_string((i + 1) * 1000);
        count_6gnrmse(graph_name, res[i], nrmse_file_nameg, str_times);
        out << str_times<< "\t" << count_time[i] / repeat_time << endl;
    }

    out.close();
    return 0;
}
