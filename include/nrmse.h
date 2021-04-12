#pragma once

#include <igraph.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
#define MOTIF4_NUM 6 //5-subgraph count
#define MOTIF5_NUM 21 //5-subgraph count
#define MOTIF6_NUM 112 //5-subgraph count
void count_4gnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
{
    string file_4g_dt = "/home/guang/graph/graphset/" + graph_name + ".4g";

    ifstream instream(file_4g_dt);

    vector<long double> real(MOTIF4_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nrmse(MOTIF4_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF4_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

//    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out.precision(52);
    out<<str_times<<" ";
    for (int i = 0; i < MOTIF4_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
//        cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}

void count_4cnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
{
    string file_4g_dt = "/home/guang/graph/graphset/" + graph_name + ".4c";

    ifstream instream(file_4g_dt);

    vector<long double> real(MOTIF4_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nrmse(MOTIF4_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        long double sum = 0;
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            sum += res[i][j];
        }
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] / sum - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF4_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

 //   cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out<<str_times<<" ";
    out.precision(52);
    for (int i = 0; i < MOTIF4_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
//        cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}

void count_5gnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
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

    vector<long double> nrmse(MOTIF5_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF5_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

//    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app); 
        out<<str_times<<" ";
    out.precision(52);
    for (int i = 0; i < MOTIF5_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
//        cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}

void count_5cnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
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

    vector<long double> nrmse(MOTIF5_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        long double sum = 0;
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            sum += res[i][j];
        }
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] / sum - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF5_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

 //   cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
        out<<str_times<<" ";
    out.precision(52);
    for (int i = 0; i < MOTIF5_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
 //       cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}

void count_6cnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
{
    string file_6g_dt = "/home/guang/graph/graphset/" + graph_name + ".6c";

    ifstream instream(file_6g_dt);

    vector<long double> real(MOTIF6_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nrmse(MOTIF6_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        long double sum = 0;
        for (int j = 0; j < MOTIF6_NUM; j++)
        {
            sum += res[i][j];
        }
        for (int j = 0; j < MOTIF6_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] / sum - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF6_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

 //   cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
        out<<str_times<<" ";
    out.precision(52);
    for (int i = 0; i < MOTIF6_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
 //       cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}


void count_6gnrmse(string graph_name, vector<vector<long double>> &res, string outfile_name,string str_times)
{
    string file_6g_dt = "/home/guang/graph/graphset/" + graph_name + ".6g";

    ifstream instream(file_6g_dt);

    vector<long double> real(MOTIF6_NUM);
    int a;
    long double b;
    while (instream >> a >> b)
    {
        real[a] = b;
    }

    vector<long double> nrmse(MOTIF6_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < MOTIF6_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] - real[j]) / real[j];
            nrmse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF6_NUM; j++)
    {
        nrmse[j] = nrmse[j] / res.size();
        nrmse[j] = sqrt(nrmse[j]);
    }

//    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app); 
        out<<str_times<<" ";
    out.precision(52);
    for (int i = 0; i < MOTIF6_NUM; ++i)
    {
        if (nrmse[i] == 1)
            nrmse[i] = 0;
//        cout << i << " " << nrmse[i] << endl;
        out << nrmse[i] << " ";
    }
    out<<endl;
    out.close();

    return;
}