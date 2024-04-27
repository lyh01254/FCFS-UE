#include "common.h"

template <typename T>
void display(vector<T> &array){
    for (int i = 0; i < array.size(); i++){
        cout << array[i] << " ";
    }
    cout << endl;
}
template void display<int>(vector<int> &array);
template void display<double>(vector<double> &array);
template void display<bool>(vector<bool> &array);

template <typename T>
void display(vector<T> &array, string name){
    cout << name << ": ";
    display(array);
}
template void display<int>(vector<int> &array, string name);
template void display<double>(vector<double> &array, string name);
template void display<bool>(vector<bool> &array, string name);

template <typename T>
void display(vector<vector<T>> &matrix){
    for (int i = 0; i < matrix.size(); i++){
        display(matrix[i]);
    }
}
template void display<int>(vector<vector<int>> &matrix);
template void display<double>(vector<vector<double>> &matrix);
template void display<bool>(vector<vector<bool>> &matrix);

template <typename T>
void display(vector<vector<T>> &matrix, string name){
    cout << name << ": " << endl;
    display(matrix);
}
template void display<int>(vector<vector<int>> &matrix, string name);
template void display<double>(vector<vector<double>> &matrix, string name);
template void display<bool>(vector<vector<bool>> &matrix, string name);

void display(int *array, int size){
    for (int i = 0; i < size; i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

void display(double *array, int size){
    for (int i = 0; i < size; i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

void display(int **matrix, int nrow, int ncol){
    for (int i = 0; i < nrow; i++){
        for (int j = 0; j < ncol; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void display(double **matrix, int nrow, int ncol){
    for (int i = 0; i < nrow; i++){
        for (int j = 0; j < ncol; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void display(int **lol, int nlist, int* list_sizes){
    for (int i = 0; i < nlist; i++){
        cout << "List" << i << ": ";
        display(lol[i], list_sizes[i]);
    }
}

void display(bool *array, int NbElements){
    for (int i = 0; i < NbElements; i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

void display(bool **matrix, int nrow, int ncol){
    for (int i = 0; i < nrow; i++){
        for (int j = 0; j < ncol; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void file_display(ofstream &file, int *array, int size){
    for (int i = 0; i < size; i++){
        file << array[i] << " ";
    }
    file << endl;
}

void copy(int *origin, int*copied, int size){
    for (int i = 0; i < size; i++){
        copied[i] = origin[i];
    }
}

void copy(double *origin, double *copied, int size){
    for (int i = 0; i < size; i++){
        copied[i] = origin[i];
    }
}