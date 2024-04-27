#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

//void display(vector<double> &array);
template <typename T> void display(vector<T> &array);
template <typename T> void display(vector<T> &array, string name);
template <typename T> void display(vector<vector<T>> &matrix);
template <typename T> void display(vector<vector<T>> &matrix, string name);
void display(int*, int);
void display(double*, int);
void display(int**, int, int);
void display(int**, int, int*);
void display(double**, int, int);
void display(bool*, int);
void display(bool**, int, int);
void file_display(ofstream&, int*, int);
void copy(int*,  int*, int);
void copy(double*, double*, int);

#endif