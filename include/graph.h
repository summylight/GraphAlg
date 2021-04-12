#pragma once

#include <igraph.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <set>
#include <random>
#include "nrmse.h"

using namespace std;
#define MOTIF4_NUM 6 //5-subgraph count
#define MOTIF5_NUM 21 //5-subgraph count
#define MOTIF6_NUM 112 //5-subgraph count
int get_random(int range)
{
    return RNG_INTEGER(0, range - 1);
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
 * \function RandStart
 * Create a startnode that  random walk could start from while obeying Stationary process 
 * 
 * \param graph the input graph
 * \return a startnode id
 * */
int RandStart(igraph_t *graph)
{
    long long v_size = igraph_vcount(graph);
    long long tmp = get_random(v_size);
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
        int pos = get_random(igraph_vector_size(&v));
        //        printf("step: %d size : %ld choose :%ld \t",i,igraph_vector_size(&v),next);
        nextNode = VECTOR(v)[pos];
        //        cout <<nextNode <<"randwalk" <<endl;
    }
    //    cout <<nextNode <<"END" <<endl;
    return nextNode;
}

int get_4graphletid(igraph_t &graph)
{
    igraph_vector_t v;
    igraph_vector_init(&v, 0);
    igraph_degree(&graph, &v, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
    int degree[4];
    vector<int> copy_degree(4);
    int id = -1;
    for (int i = 0; i < 4; i++)
    {
        degree[i] = VECTOR(v)[i];
    }
    copy_degree[0] = degree[0];
    copy_degree[1] = degree[1];
    copy_degree[2] = degree[2];
    copy_degree[3] = degree[3];
    sort(degree, degree + 4);

    if (degree[0] == 1 && degree[1] == 1 && degree[2] == 1 && degree[3] == 3)
    {
        id = 0;
    }
    else if (degree[0] == 1 && degree[1] == 1 && degree[2] == 2 && degree[3] == 2)
    {
        id = 1;
    }
    else if (degree[0] == 1 && degree[1] == 2 && degree[2] == 2 && degree[3] == 3)
    {
        id = 2;
    }
    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 2 && degree[3] == 2)
    {
        id = 3;
    }
    else if (degree[0] == 2 && degree[1] == 2 && degree[2] == 3 && degree[3] == 3)
    {
        id = 4;
    }
    else if (degree[0] == 3 && degree[1] == 3 && degree[2] == 3 && degree[3] == 3)
    {
        id = 5;
    }
    return id;
}


int get_5graphletid(igraph_t &graph)
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

void init_igraph6(vector<igraph_t> &graph)
{
    graph.resize(112);
    igraph_small(&graph[0], 6, IGRAPH_UNDIRECTED, 0, 5, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[1], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[2], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[3], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[4], 6, IGRAPH_UNDIRECTED, 0, 4, 1, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[5], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[6], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[7], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[8], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[9], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[10], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[11], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[12], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[13], 6, IGRAPH_UNDIRECTED, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[14], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 5, -1);
    igraph_small(&graph[15], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[16], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[17], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, -1);
    igraph_small(&graph[18], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 4, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[19], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[20], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[21], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 4, 5, -1);
    igraph_small(&graph[22], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[23], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, -1);
    igraph_small(&graph[24], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[25], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[26], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 4, 5, -1);
    igraph_small(&graph[27], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[28], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[29], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[30], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[31], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[32], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 5, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[33], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 5, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[34], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[35], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[36], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[37], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[38], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[39], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[40], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[41], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[42], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[43], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[44], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[45], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 5, -1);
    igraph_small(&graph[46], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 5, 3, 5, -1);
    igraph_small(&graph[47], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[48], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[49], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[50], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[51], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[52], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[53], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[54], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[55], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[56], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[57], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[58], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[59], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[60], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[61], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[62], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[63], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[64], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[65], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[66], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[67], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 1, 3, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[68], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[69], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[70], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, -1);
    igraph_small(&graph[71], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[72], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[73], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[74], 6, IGRAPH_UNDIRECTED, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[75], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[76], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[77], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[78], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[79], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[80], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[81], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[82], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[83], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[84], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[85], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[86], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[87], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[88], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, -1);
    igraph_small(&graph[89], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 3, 5, -1);
    igraph_small(&graph[90], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[91], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[92], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[93], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[94], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[95], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[96], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[97], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[98], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[99], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[100], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[101], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[102], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[103], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[104], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, -1);
    igraph_small(&graph[105], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 4, 5, -1);
    igraph_small(&graph[106], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[107], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 5, 4, 5, -1);
    igraph_small(&graph[108], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, -1);
    igraph_small(&graph[109], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[110], 6, IGRAPH_UNDIRECTED, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    igraph_small(&graph[111], 6, IGRAPH_UNDIRECTED, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 2, 1, 3, 1, 4, 1, 5, 2, 3, 2, 4, 2, 5, 3, 4, 3, 5, 4, 5, -1);
    return;
}
vector<vector<bool> > init_6graphletid(){
        igraph_t graph5[21];
    
    igraph_small(&graph5[0],5,IGRAPH_UNDIRECTED,0,4,1,4,2,4,3,4,-1);
    igraph_small(&graph5[1],5,IGRAPH_UNDIRECTED,0,3,0,4,1,4,2,4,-1);
    igraph_small(&graph5[2],5,IGRAPH_UNDIRECTED,0,3,0,4,1,4,2,4,3,4,-1);
    igraph_small(&graph5[3],5,IGRAPH_UNDIRECTED,0,3,0,4,1,3,1,4,2,4,-1);
    igraph_small(&graph5[4],5,IGRAPH_UNDIRECTED,0,3,0,4,1,3,2,4,3,4,-1);
    igraph_small(&graph5[5],5,IGRAPH_UNDIRECTED,0,3,0,4,1,3,1,4,2,4,3,4,-1);
    igraph_small(&graph5[6],5,IGRAPH_UNDIRECTED,0,3,0,4,1,3,1,4,2,3,2,4,-1);
    igraph_small(&graph5[7],5,IGRAPH_UNDIRECTED,0,3,0,4,1,3,1,4,2,3,2,4,3,4,-1);
    igraph_small(&graph5[8],5,IGRAPH_UNDIRECTED,0,2,0,4,1,3,1,4,-1);
    igraph_small(&graph5[9],5,IGRAPH_UNDIRECTED,0,2,0,4,1,3,1,4,2,4,-1);
    igraph_small(&graph5[10],5,IGRAPH_UNDIRECTED,0,2,0,4,1,3,1,4,2,4,3,4,-1);
    igraph_small(&graph5[11],5,IGRAPH_UNDIRECTED,0,2,0,3,1,3,1,4,2,4,-1);
    igraph_small(&graph5[12],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,3,1,4,2,4,-1);
    igraph_small(&graph5[13],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,3,1,4,2,4,3,4,-1);
    igraph_small(&graph5[14],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,4,2,3,2,4,-1);
    igraph_small(&graph5[15],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,4,2,3,2,4,3,4,-1);
    igraph_small(&graph5[16],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,3,1,4,2,3,2,4,3,4,-1);
    igraph_small(&graph5[17],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,2,1,3,1,4,2,4,-1);
    igraph_small(&graph5[18],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,2,1,3,1,4,2,4,3,4,-1);
    igraph_small(&graph5[19],5,IGRAPH_UNDIRECTED,0,2,0,3,0,4,1,2,1,3,1,4,2,3,2,4,3,4,-1);
    igraph_small(&graph5[20],5,IGRAPH_UNDIRECTED,0,1,0,2,0,3,0,4,1,2,1,3,1,4,2,3,2,4,3,4,-1);
    vector<igraph_t> graph6;
    init_igraph6(graph6);

    
    vector<vector<bool>> a(112,vector<bool>(21));
    for(int i = 0; i<112;i++)
        for(int j =0;j<21;j++){
            igraph_bool_t iso;
            igraph_subisomorphic(&graph6[i],&graph5[j],&iso);
            if(iso) a[i][j]=1;
            else a[i][j]=0;
        }
    return a;
}
