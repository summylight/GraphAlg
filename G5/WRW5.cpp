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

    string prestr = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time);

    igraph_t G;
    gettimeofday(&start, NULL);
    gettimeofday(&realstart, NULL);
    igraph_read_graph_ncol(&G, instream, NULL, 0, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
    gettimeofday(&end, NULL);
    dur = dur + (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
    cout << "read time : " << dur << endl;
    dur = 0;

    vector<int> W_constant = {24, 2, 4, 4, 2, 4, 12, 12, 2, 4, 8, 10, 14, 20, 8, 12, 36, 28, 48, 72, 120};

    for (int iii = 0; iii < repeat_time; ++iii)
    {
        RNG_BEGIN();
        int startnode = RandStart(&G);
        igraph_vector_t walknodes;
        igraph_vector_init(&walknodes, 0);
        dur=0;

        //得到收敛后的路径
        igraph_random_walk(&G, &walknodes, startnode, IGRAPH_ALL, given_time+4, IGRAPH_RANDOM_WALK_STUCK_ERROR);

        igraph_vector_t degrees;
        igraph_vector_init(&degrees, given_time+4);
        igraph_vs_t vs;
        igraph_vs_vector(&vs, &walknodes);
        igraph_degree(&G, &degrees, vs, IGRAPH_ALL, IGRAPH_NO_LOOPS);

        set<int> s;
        vector<long double> cc(21);

        igraph_vector_t neis;
        igraph_vector_init(&neis, 0);


        for (int pos = 4; pos < given_time+4; pos += jump_len)
        {
            // 找path 为5的子图
            s.clear();
            bool ContainDuplicate = 0;
            vector<int> vc;
            for (int k = 0; k < 5; k++)
            {
                vc.insert(vc.end(), VECTOR(walknodes)[pos - 4 + k]);
                if (!s.insert(VECTOR(walknodes)[pos - 4 + k]).second)
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
                        if (get_edge_status(G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 2] * VECTOR(degrees)[pos - 3];
                int gid = get_5graphletid(subgraph);
                if (gid == 0 || gid == 1 || gid == 2)
                    cout << "here" << endl;
                cc[gid] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }

            // 找path 为4的子图
            ContainDuplicate = 0;
            s.clear();
            vc.clear();
            for (int k = 0; k < 4; k++)
            {
                vc.insert(vc.end(), VECTOR(walknodes)[pos - 3 + k]);
                if (!s.insert(VECTOR(walknodes)[pos - 3 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }
            igraph_neighbors(&G, &neis, VECTOR(walknodes)[pos - 2], IGRAPH_ALL);
            int neissize = igraph_vector_size(&neis);
            int user = RNG_INTEGER(0, neissize - 1);
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
                        if (get_edge_status(G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 2] * VECTOR(degrees)[pos - 2];
                int id = get_5graphletid(subgraph);
                if (id == 2 || id == 1)
                    cc[id] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }

            // 找path 为3的子图
            s.clear();
            ContainDuplicate = 0;
            vc.clear();
            for (int k = 0; k < 3; k++)
            {
                vc.insert(vc.end(), VECTOR(walknodes)[pos - 2 + k]);
                if (!s.insert(VECTOR(walknodes)[pos - 2 + k]).second)
                {
                    ContainDuplicate = 1;
                    break;
                }
            }
            igraph_neighbors(&G, &neis, VECTOR(walknodes)[pos - 1], IGRAPH_ALL);
            neissize = igraph_vector_size(&neis);
            int user1 = RNG_INTEGER(0, neissize - 1);
            int user2 = RNG_INTEGER(0, neissize - 1);
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
                        if (get_edge_status(G, vc[ii], vc[j]))
                        {
                            igraph_vector_push_back(&ivc, ii);
                            igraph_vector_push_back(&ivc, j);
                        }
                    }
                }
                igraph_create(&subgraph, &ivc, 0, IGRAPH_UNDIRECTED);
                long double deg = (long double)VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 1] * VECTOR(degrees)[pos - 1];
                int id = get_5graphletid(subgraph);
                if (id == 0)
                    cc[id] += deg;
                igraph_destroy(&subgraph);
                igraph_vector_destroy(&ivc);
            }
            if ((pos - 3) % 1000 == 0)
            {

                gettimeofday(&end, NULL);
                dur = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;

                vector<long double> ans(MOTIF5_NUM);
                long double sum = 0;
                for (int i = 0; i < MOTIF5_NUM; ++i)
                    sum += (ans[i] = (cc[i] / W_constant[i]));

                for (int i = 0; i < MOTIF5_NUM; ++i)
                {
                    ans[i] /= sum;
                }
                res[(pos-4) / 1000].push_back(ans);
                count_time[(pos-4) / 1000] += dur;
            }
        }
        igraph_vector_destroy(&walknodes);
        igraph_vector_destroy(&degrees);
    }
     string time_file_name = graph_name + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + ".timewrw5";
    ofstream out(time_file_name);
    printf("Sample Use Time: %f s per sample Use Time:%f s\n", dur, dur / repeat_time);
    cout << "SSRW and NMSRE is writing to " << prestr << ".cwrw5 and " << prestr << ".cnmrsewrw5" << endl;
    gettimeofday(&realend, NULL);
    dur = (realend.tv_sec - realstart.tv_sec) + (double)(realend.tv_usec - realstart.tv_usec) / 1000000.0;
    printf("All Time:%f\n", dur); //count time
    string nmrse_file_namec = graph_name + "_" + to_string(given_time) + "_" + to_string(jump_len) + "_" + to_string(repeat_time) + ".cnmrsewrw5";
    for (int i = 0; i < given_time / 1000; i++)
    {
        string str_times = to_string((i + 1) * 1000);
        count_5cnmrse(graph_name, res[i], nmrse_file_namec, str_times);

        out << (i + 1) * 1000 << "\t" << count_time[i] / repeat_time << endl;
    }
    out.close();
    return 0;
}
