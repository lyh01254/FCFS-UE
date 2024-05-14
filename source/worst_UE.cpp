#include "myIOS.h"
#include "common.h"
#include "curve.h"

using namespace std;

int NbHotels, NbTypes;
double gamma;
vector<double> demand;
vector<vector<double> > capacity;
vector<double> cost;
vector<double> Capacity_by_k;
vector<bool> is_excess;


string root = "data/instance/";

int main(int argc, char* argv[]){
    string file_name;
    if (argc > 1){
        file_name = root + argv[1];
    }else{
        file_name = "data/demo.csv";
    }
    try{
        read(NbHotels, file_name, 0, 0);
        read(NbTypes, file_name, 0, 1);
        read(gamma, file_name, 0, 2);
        read(demand, file_name, 1, 0, 1, NbTypes-1);
        read(cost, file_name, 2, NbTypes, 1+NbHotels, NbTypes);
        read(capacity, file_name, 2, 0, 1+NbHotels, NbTypes-1);
    }catch(const char* msg){
        cerr << msg << endl;
        exit(1);
    }
    for (int k = 0; k < NbTypes; k++){
        Capacity_by_k.push_back(0);
        for (int i = 0; i < NbHotels; i++){
            Capacity_by_k[k] += capacity[i][k];
        }
        if (Capacity_by_k[k] <= demand[k]){
            is_excess.push_back(true);
        } else {
            is_excess.push_back(false);
        }
    }
    display(demand, "Demand");
    display(cost, "Cost");
    cout << "gamma = " << gamma << endl;
    display(capacity, "Capacity");
    display(Capacity_by_k, "Capacity_by_k");
    cout << "Data input passes." << endl;

    //gamma = 1;

    std::vector<Curve> v(NbTypes);
    find_v(v, demand, capacity, cost, gamma);
    for (int k = 0; k < NbTypes; k++){
        v[k].display();
    }

    return 0;
}

