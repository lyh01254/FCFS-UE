#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//void display(vector<double> &array);
template <typename T> void display(std::vector<T> &array);
template <typename T> void display(std::vector<T> &array, std::string name);
template <typename T> void display(std::vector<std::vector<T> > &matrix);
template <typename T> void display(std::vector<std::vector<T> > &matrix, std::string name);
void display(int*, int);
void display(double*, int);
void display(int**, int, int);
void display(int**, int, int*);
void display(double**, int, int);
void display(bool*, int);
void display(bool**, int, int);
void file_display(std::ofstream&, int*, int);
void copy(int*,  int*, int);
void copy(double*, double*, int);

#endif