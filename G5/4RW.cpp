#include "../include/graph.h"


/**
 * @brief 基于4cycle的方法
 *    
 * 
 *      p 
 *    |   \ 
 *   |      \
 *  |         \
 * 1------2----3-------4-------5-------6
 * 
 * @param argc ./SSRW5 amazon/catster/dblp/enron/gnutella/live/pokec run_time_single_turn jump_len(1) repeat_time
 * 
 * @param argv 
 * @return int 
 */

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
     int W_constant[] = {0, 0, 0, 2, 0, 2, 12, 12, 0, 0, 0, 0, 4, 8, 2, 6, 24, 16, 32, 60, 120};

    for (int iii = 0; iii < repeat_time; ++iii)
    {
        igraph_rng_seed(igraph_rng_default(), rd());
        int startid = RandStart(&G);
        vector<long double> count(MOTIF5_NUM); //count times all subgraph may appear

        gettimeofday(&start, NULL);
        igraph_vector_t walknodes;
        igraph_vector_init(&walknodes, 0);
        dur = 0;
        igraph_random_walk(&G, &walknodes, startid, IGRAPH_ALL, given_time+3, IGRAPH_RANDOM_WALK_STUCK_RETURN);

        igraph_vs_t vs;
        igraph_vs_vector(&vs, &walknodes);
        igraph_vector_t degrees;
        igraph_vector_init(&degrees, 0);
        igraph_degree(&G, &degrees, vs, IGRAPH_ALL, IGRAPH_NO_LOOPS);
        igraph_vs_destroy(&vs);
        igraph_vector_t nei1, nei2, intersection;

        for (int pos = 2; pos < given_time +2; pos+=jump_len)
        {
            igraph_vector_init(&nei1, 0);
            igraph_neighbors(&G, &nei1, VECTOR(walknodes)[pos - 2], IGRAPH_ALL);

            igraph_vector_init(&nei2, 0);
            igraph_neighbors(&G, &nei2, VECTOR(walknodes)[pos], IGRAPH_ALL);

            igraph_vector_init(&intersection, 0);
            igraph_vector_intersect_sorted(&nei1, &nei2, &intersection);

            int node5_user = RNG_INTEGER(0, igraph_vector_size(&intersection) - 1);
            int node5 = VECTOR(intersection)[node5_user];

            vector<int> node;
            for (int k = pos - 2; k < pos + 2; k++)
            {
                node.push_back(VECTOR(walknodes)[k]);
            }
            node.push_back(node5);
            set<int> it(node.begin(), node.end());
            if (it.size() == 5)
            {
                long double dg_prod = igraph_vector_size(&intersection) * VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos];
                igraph_t subgraph;
                igraph_vector_t vc;
                igraph_vector_init(&vc, 0);
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = i + 1; j < 5; ++j)
                    {
                        if (get_edge_status(G, node[i], node[j]))
                        {
                            igraph_vector_push_back(&vc, i);
                            igraph_vector_push_back(&vc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &vc, 0, IGRAPH_UNDIRECTED);
                int gid = get_5graphletid(subgraph);
                count[gid] += dg_prod;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&vc);
            }
            if((pos-1)%1000==0){

        gettimeofday(&end, NULL);
        dur = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;

        vector<long double> ans(MOTIF5_NUM);
        for (int i = 0; i < MOTIF5_NUM; ++i)
            if(W_constant[i]!=0) ans[i] = ((count[i] / W_constant[i]) / (given_time - 3)) * igraph_ecount(&G) * 2;

                res[(pos-2) / 1000].push_back(ans);
                count_time[(pos-2) / 1000] += dur;
            }
        }

        igraph_vector_destroy(&walknodes);
        igraph_vector_destroy(&degrees);

    }
    string time_file_name = graph_name + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + ".time4rw";
    ofstream out(time_file_name);
    printf("Sample Use Time: %f s per sample Use Time:%f s\n",dur, dur / repeat_time);
    cout << "SSRW and NMSRE is writing to " << s << ".g4rw and " << s << ".gnrmse4rw" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%f\n", dur);
    string nmrse_file_nameg = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + ".gnrmse4rw";
    for (int i = 0; i < given_time / 1000; i++)
    {
        string str_times = to_string((i + 1) * 1000);
        count_5gnmrse(graph_name, res[i], nmrse_file_nameg, str_times);

        out << (i + 1) * 1000 << "\t" << count_time[i] / repeat_time << endl;
    }
    out.close();

    return 0;
}
