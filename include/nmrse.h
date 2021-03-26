#pragma once

#include <igraph.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
#define MOTIF4_NUM 6
#define MOTIF5_NUM 21
void count_4gnmrse(string graph_name, vector<vector<long double>> &res, string outfile_name)
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

    vector<long double> nmrse(MOTIF4_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] - real[j]) / real[j];
            nmrse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF4_NUM; j++)
    {
        nmrse[j] = nmrse[j] / res.size();
        nmrse[j] = sqrt(nmrse[j]);
    }

    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out.precision(52);
    for (int i = 0; i < MOTIF4_NUM; ++i)
    {
        if (nmrse[i] == 1)
            nmrse[i] = 0;
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}

void count_4cnmrse(string graph_name, vector<vector<long double>> &res, string outfile_name)
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

    vector<long double> nmrse(MOTIF4_NUM);

    for (int i = 0; i < res.size(); i++)
    {
        double sum = 0;
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            sum += res[i][j];
        }
        for (int j = 0; j < MOTIF4_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] / sum - real[j]) / real[j];
            nmrse[j] += cha * cha;
        }
    }

    for (int j = 0; j < MOTIF4_NUM; j++)
    {
        nmrse[j] = nmrse[j] / res.size();
        nmrse[j] = sqrt(nmrse[j]);
    }

    cout << "Now writing to " << outfile_name << endl;
    ofstream out(outfile_name, std::ios_base::app);
    out.precision(52);
    for (int i = 0; i < MOTIF4_NUM; ++i)
    {
        if (nmrse[i] == 1)
            nmrse[i] = 0;
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}

void count_5gnmrse(string graph_name, vector<vector<long double>> &res, string outfile_name)
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
            if (real[j] == 0)
                continue;
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
    out.precision(52);
    for (int i = 0; i < MOTIF5_NUM; ++i)
    {
        if (nmrse[i] == 1)
            nmrse[i] = 0;
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}

void count_5cnmrse(string graph_name, vector<vector<long double>> &res, string outfile_name)
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
        double sum = 0;
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            sum += res[i][j];
        }
        for (int j = 0; j < MOTIF5_NUM; j++)
        {
            if (real[j] == 0)
                continue;
            long double cha = (res[i][j] / sum - real[j]) / real[j];
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
        if (nmrse[i] == 1)
            nmrse[i] = 0;
        cout << i << " " << nmrse[i] << endl;
        out << i << " " << nmrse[i] << endl;
    }
    out.close();

    return;
}