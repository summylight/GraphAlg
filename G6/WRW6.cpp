#include "../include/graph.h"

using namespace std;

int main(int argc, char *argv[])
{
    random_device rd;

    long double dur = 0;
    struct timeval start, end, realstart, realend;
    if (argc < 5)
    {
        printf("The pramament of the procedure is like ./WRW5 amazon/catster/dblp/enron/gnutella/live/pokec run_time_single_turn jump_len(1) repeat_time\n");
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

    string prestr = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time);

    igraph_t G;
    gettimeofday(&start, NULL);
    gettimeofday(&realstart, NULL);
    igraph_read_graph_ncol(&G, instream, NULL, 0, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
    gettimeofday(&end, NULL);
    dur = dur + (end.tv_sec - start.tv_sec) + (long double)(end.tv_usec - start.tv_usec) / 1000000.0;
    cout << "read time : " << dur << endl;
    dur = 0;
    igraph_t g1;
    igraph_t g112;
    igraph_small(&g1, 6, IGRAPH_UNDIRECTED, 0, 5, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&g112, 6, IGRAPH_UNDIRECTED, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    int W_constant[112] = {120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 720};
    for (int iii = 0; iii < repeat_time; ++iii)
    {
        RNG_BEGIN();
        int startnode = RandStart(&G);
        igraph_vector_t walknodes;
        igraph_vector_init(&walknodes, 0);
        dur = 0;

        //得到收敛后的路径
        gettimeofday(&start, NULL);
        igraph_random_walk(&G, &walknodes, startnode, IGRAPH_ALL, given_time + 5, IGRAPH_RANDOM_WALK_STUCK_ERROR);

        igraph_vector_t degrees;
        igraph_vector_init(&degrees, given_time + 5);
        igraph_vs_t vs;
        igraph_vs_vector(&vs, &walknodes);
        igraph_degree(&G, &degrees, vs, IGRAPH_ALL, IGRAPH_NO_LOOPS);

        set<int> s;
        vector<long double> cc(112);

        igraph_vector_t neis;
        igraph_vector_init(&neis, 0);
        vector<int> vc(6);
        for (int pos = 5; pos < given_time + 5; pos += jump_len)
        {
            // 找path 为6的子图
            s.clear();
            bool ContainDuplicate = 0;

            for (int k = 0; k < 6; k++)
            {
                vc[k] = VECTOR(walknodes)[pos - 5 + k];
                if (!s.insert(VECTOR(walknodes)[pos - 5 + k]).second)
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
                for (int ii = 0; ii < 5; ++ii)
                {
                    for (int j = ii + 1; j < 6; ++j)
                    {
                        if (get_edge_status(G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 2] * VECTOR(degrees)[pos - 3] * VECTOR(degrees)[pos - 4];
                igraph_bool_t iso;
                igraph_isomorphic(&g112, &subgraph, &iso);
                if (iso)
                    cc[111] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }

            // 找path 为3的子图
            s.clear();
            ContainDuplicate = 0;
            for (int k = 0; k < 3; k++)
            {
                vc[k]=VECTOR(walknodes)[pos - 2 + k];
                if (!s.insert(VECTOR(walknodes)[pos - 2 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }
            igraph_neighbors(&G, &neis, VECTOR(walknodes)[pos - 1], IGRAPH_ALL);
            int neissize = igraph_vector_size(&neis);
            int user1 = RNG_INTEGER(0, neissize - 1);
            int user2 = RNG_INTEGER(0, neissize - 1);
            int user3 = RNG_INTEGER(0, neissize - 1);
            vc[3]=VECTOR(neis)[user1];
            vc[4]=VECTOR(neis)[user2];
            vc[5]=VECTOR(neis)[user3];
            if (!s.insert(vc[4]).second)
            {
                ContainDuplicate = 1;
            }
            if (!s.insert(vc[5]).second)
            {
                ContainDuplicate = 1;
            }
            if (!s.insert(vc[3]).second)
            {
                ContainDuplicate = 1;
            }
            if (!ContainDuplicate)
            {
                igraph_t subgraph;

                igraph_vector_t ivc;
                igraph_vector_init(&ivc, 0);
                for (int ii = 0; ii < 5; ++ii)
                {
                    for (int j = ii + 1; j < 6; ++j)
                    {
                        if (get_edge_status(G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 1];
                igraph_bool_t iso;
                igraph_isomorphic(&g1, &subgraph, &iso);
                if (iso)
                    cc[0] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }
            if ((pos - 4) % 1000 == 0)
            {

                gettimeofday(&end, NULL);
                dur = (end.tv_sec - start.tv_sec) + (long double)(end.tv_usec - start.tv_usec) / 1000000.0;

                vector<long double> ans(MOTIF6_NUM);
                for (int i = 0; i < MOTIF6_NUM; ++i)
                    if (W_constant[i]>0)
                        ans[i] = ((cc[i] * 2 * igraph_ecount(&G)) / W_constant[i]) / (pos - 4);

                res[(pos - 5) / 1000].push_back(ans);
                count_time[(pos - 5) / 1000] += dur;
            }
        }
        igraph_vector_destroy(&walknodes);
        igraph_vector_destroy(&degrees);
    }
    string time_file_name = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "timewrw6.txt";
    ofstream out(time_file_name);
    printf("Sample Use Time: %Lf s per sample Use Time:%Lf s\n", dur, dur / repeat_time);
    cout << "SSRW and NMSRE is writing to " << prestr << "cwrw6.txt" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (long double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%Lf\n", dur); //count time
    string nrmse_file_nameg = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + "gwrw6.txt";
    for (int i = 0; i < given_time / 1000; i++)
    {
        string str_times = to_string((i + 1) * 1000);
        count_6gnrmse(graph_name, res[i], nrmse_file_nameg, str_times);
        out << (i + 1) * 1000 << "\t" << count_time[i] / repeat_time << endl;
    }
    out.close();
    return 0;
}
