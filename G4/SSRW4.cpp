#include "../include/graph.h"

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
    vector<vector<vector<long double>>> res; //记录1000单位的数据
    vector<double> count_time;
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
    dur = dur + (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
    cout << "read time : " << dur << endl;
    dur = 0;

    int W_constant[] = {6, 2, 10, 8, 24, 48};

    for (int iii = 0; iii < repeat_time; ++iii)
    {
        igraph_rng_seed(igraph_rng_default(), rd());
        int startid = RandStart(&G);
        long double count[MOTIF4_NUM]; //count times all subgraph may appear
        int run_times = 0;

        for (int i = 0; i < MOTIF4_NUM; ++i)
            count[i] = 0;
        gettimeofday(&start, NULL);
        vector<int> node(4), user(4), degree(4);
        igraph_vector_t *nodeneigh[3];
        igraph_lazy_adjlist_t adj;
        IGRAPH_CHECK(igraph_lazy_adjlist_init(&G, &adj, IGRAPH_ALL, IGRAPH_DONT_SIMPLIFY));
        IGRAPH_FINALLY(igraph_lazy_adjlist_destroy, &adj);
        /*         while(1){
            sample_times++; */
        dur = 0;
        for (; run_times < given_time; ++run_times)
        {
            //            cout << " count subgraph time  : " << run_times <<endl;
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

            int dg_prod = degree[1] * (degree[1] + degree[2]);
            set<int> it(node.begin(), node.end());
            if (it.size() == 4)
            {
                igraph_t subgraph, subgraph4;
                igraph_vector_t vc;
                igraph_vector_init(&vc, 0);
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = i + 1; j < 4; ++j)
                    {
                        if (igraph_vector_binsearch2(nodeneigh[i], node[j]))
                        {
                            igraph_vector_push_back(&vc, i);
                            igraph_vector_push_back(&vc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &vc, 0, IGRAPH_UNDIRECTED);
                int gid = get_4graphletid(subgraph);
                count[gid] = count[gid] + dg_prod;
                igraph_destroy(&subgraph);
            }
            startid = RandWalk(&G, jump_len, node[0]);
            if ((run_times + 1) % 1000 == 0)
            {
                gettimeofday(&end, NULL);
                dur = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;

                vector<long double> ans(MOTIF4_NUM);
                for (int i = 0; i < MOTIF4_NUM; ++i)
                    ans[i] = ((count[i] / W_constant[i]) / (run_times + 1)) * igraph_ecount(&G) * 2;

                res[run_times / 1000].push_back(ans);
                count_time[run_times / 1000] += dur;
            }
        }
        igraph_lazy_adjlist_destroy(&adj);
        IGRAPH_FINALLY_CLEAN(1);
    }
    //时间统计
    string time_file_name = graph_name + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "timessrw4.txt";
    ofstream out(time_file_name);
    printf("Sample Use Time: %f s per sample Use Time:%f s\n", dur, dur / repeat_time);
    cout << "SSRW and NMSRE is writing to " << s << "gssrw4.txt" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%f\n", dur); //count time
    string nrmse_file_nameg = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "gssrw4.txt";
    string nrmse_file_namec = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "cssrw4.txt";
    for (int i = 0; i < given_time / 1000; i++)
    {
        string str_times = to_string((i + 1) * 1000);
        count_4gnrmse(graph_name, res[i], nrmse_file_nameg, str_times);
        count_4cnrmse(graph_name, res[i], nrmse_file_namec, str_times);

        out << (i + 1) * 1000 << "\t" << count_time[i] / repeat_time << endl;
    }
    return 0;
}
