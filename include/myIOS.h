#ifndef MYIOS_H
#define MYIOS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void read(int &para, std::string file_name, int r_index, int c_index);

void read(double &para, std::string file_name, int r_index, int c_index);

void read(std::vector<int> &para, std::string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(std::vector<double> &para, std::string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(std::vector<std::vector<int> > &para, std::string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

void read(std::vector<std::vector<double> > &para, std::string file_name, int r_index0, int c_index0, int r_index1, int c_index1);

#endif

