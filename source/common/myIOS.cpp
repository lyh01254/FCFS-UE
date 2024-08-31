#include"myIOS.h"
using namespace std;

void read(int &para, string file_name, int r_index, int c_index){
    //* This function reads the value for an integer parameter [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    for (int i = 0; i < r_index+1; i++){
        if (!getline(infile, line_string)){
            throw "r_index exceeds range!";
        };
    }
    
    stringstream ss(line_string);
    string str;
    for (int i = 0; i < c_index+1; i++){
        if (!getline(ss, str, ',')){
            throw "c_index exceeds range!";
        };
    }
    para = stoi(str);
    infile.close();
}

void read(double &para, string file_name, int r_index, int c_index){
    //* This function reads the value for a scalar parameter [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    for (int i = 0; i < r_index+1; i++){
        if (!getline(infile, line_string)){
            throw "r_index exceeds range!";
        };
    }
    stringstream ss(line_string);
    string str;
    for (int i = 0; i < c_index+1; i++){
        if (!getline(ss, str, ',')){
            throw "c_index exceeds range!";
        };
    }
    para = stod(str);
    infile.close();
}

void read(vector<int> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1){
    //* This function reads the value for an integer parameter array [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    string str;
    for (int i = 0; i < r_index1+1; i++){
        if (getline(infile, line_string)){
            if (i >= r_index0){
                stringstream ss(line_string);
                for (int j = 0; j < c_index1+1; j++){
                    if (getline(ss, str, ',')){
                        if (j >= c_index0){
                            para.push_back(stoi(str));
                        }
                    } else {
                        throw "c_index exceeds range!";
                    }
                }
            }
        }else{
            throw "r_index exceeds range!";
        }
    }
}

void read(vector<double> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1){
    //* This function reads the value for an integer parameter array [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    string str;
    for (int i = 0; i < r_index1+1; i++){
        if (getline(infile, line_string)){
            if (i >= r_index0){
                stringstream ss(line_string);
                for (int j = 0; j < c_index1+1; j++){
                    if (getline(ss, str, ',')){
                        if (j >= c_index0){
                            para.push_back(stod(str));
                        }
                    } else {
                        throw "c_index exceeds range!";
                    }
                }
            }
        }else{
            throw "r_index exceeds range!";
        }
    }
}

void read(vector<vector<int>> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1){
    //* This function reads the value for an integer parameter matrix [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    string str;
    for (int i = 0; i < r_index1+1; i++){
        if (getline(infile, line_string)){
            if (i >= r_index0){
                vector<int> row;
                stringstream ss(line_string);
                for (int j = 0; j < c_index1+1; j++){
                    if (getline(ss, str, ',')){
                        if (j >= c_index0){
                            row.push_back(stoi(str));
                        }
                    } else {
                        throw "c_index exceeds range!";
                    }
                }
                para.push_back(row);
            }
        }else{
            throw "r_index exceeds range!";
        }
    }
}

void read(vector<vector<double>> &para, string file_name, int r_index0, int c_index0, int r_index1, int c_index1){
    //* This function reads the value for a scalar parameter matrix [para] from .csv files
    //* r_index and c_index start from 0
    ifstream infile;
    infile.open(file_name, ios::in);
    string line_string;
    string str;
    for (int i = 0; i < r_index1+1; i++){
        if (getline(infile, line_string)){
            if (i >= r_index0){
                vector<double> row;
                stringstream ss(line_string);
                for (int j = 0; j < c_index1+1; j++){
                    if (getline(ss, str, ',')){
                        if (j >= c_index0){
                            row.push_back(stod(str));
                        }
                    } else {
                        throw "c_index exceeds range!";
                    }
                }
                para.push_back(row);
            }
        }else{
            throw "r_index exceeds range!";
        }
    }
}

void set_header(std::string file_name, std::vector<std::string> fields){
    std::ofstream file;
    file.open(file_name, std::ios::out|std::ios::trunc);
    if (file.is_open()){
        std::string header;
        for (int i = 0; i < fields.size()-1; i++){
            header = header + fields[i] + ",";
        }
        header += fields.back();
        file << header << std::endl;
        file.close();
    } else {
        std::cout << "Failed to open file " << file_name << endl; 
    }
}

void append_row(std::string file_name, std::vector<std::string> row){
    std::ofstream file;
    file.open(file_name, std::ios::out|std::ios::app);
    if (file.is_open()){
        std::string row_string;
        for (int i = 0; i < row.size()-1; i++){
            row_string = row_string + row[i] +",";
        }
        row_string += row.back();
        file << row_string << std::endl;
        file.close();
    } else {
        std::cout << "Failed to open file " << file_name << endl; 
    }
}

void append_matrix(std::string file_name, std::vector<std::vector<std::string>> matrix){
    std::ofstream file;
    file.open(file_name, std::ios::out|std::ios::app);
    if (file.is_open()) {
        for (int i = 0; i < matrix.size(); i++){
            std::string row;
            for (int j = 0; j < matrix[i].size()-1; j++){
                row = row + matrix[i][j] + ",";
            }
            row += matrix[i].back();
            file << row << endl;
        }
        file.close();
    } else {
        std::cout << "Failed to open file " << file_name << endl; 
    }
}

