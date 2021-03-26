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
#include "nmrse.h"

using namespace std;
#define MOTIF4_NUM 6 //5-subgraph count
#define MOTIF5_NUM 21 //5-subgraph count
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
