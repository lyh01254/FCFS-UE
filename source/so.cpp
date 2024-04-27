#include "myIOS.h"
#include "common.h"

int NbHotels, NbTypes;
double gamma;
vector<double> demand;
vector<vector<double>> capacity;
vector<double> cost;

string file_name = "data/demo.csv";

int main(){
    try{
        read(NbHotels, file_name, 6, 0);
        read(NbTypes, file_name, 0, 1);
        read(gamma, file_name, 0, 2);
        read(demand, file_name, 1, 0, 1, NbHotels);
        read(cost, file_name, 2, NbTypes, 1+NbHotels, NbTypes);
        read(capacity, file_name, 2, 0, 1+NbHotels, NbTypes-1);
    }catch(const char* msg){
        cerr << msg << endl;
        exit(1);
    }
    display(demand, "Demand");
    display(cost, "Cost");
    display(capacity, "Capacity");
    return 0;
}
