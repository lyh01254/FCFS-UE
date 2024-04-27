#ifndef MYIOS_H
#define MYIOS_H
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;

void read(int &para, string file_name, int r_index, int c_index);

void read(double &para, string file_name, int r_index, int c_index);

void read(vector<int> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(vector<double> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(vector<vector<int>> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(vector<vector<double>> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

#endif

