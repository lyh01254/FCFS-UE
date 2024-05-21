#include "myIOS.h"
#include "common.h"

using namespace std;

int NbHotels, NbTypes;
double gamma;
vector<double> demand;
vector<vector<double> > capacity;
vector<double> cost;
vector<double> Capacity_by_k;
vector<bool> is_excess;
vector<int> excess_types;
vector<int> surplus_types;
double direct_misplace;
double total_surplus;
vector<double> difference;
double best_surplus;


string root = "data/totally_random/";

double excess_tsp();
double surplus_tsp();


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

    direct_misplace = 0;
    total_surplus = 0;
    double excess_assignment_cost = 0;
    
    for (int k = 0; k < NbTypes; k++){
        Capacity_by_k.push_back(0);
        double temp = 0;
        for (int i = 0; i < NbHotels; i++){
            Capacity_by_k[k] += capacity[i][k];
            temp += capacity[i][k] * cost[i];
        }
        difference.push_back(demand[k] - Capacity_by_k[k]);
        if (difference.back() >= 0){
            is_excess.push_back(true);
            excess_types.push_back(k);
            direct_misplace += difference.back();
            excess_assignment_cost += temp;
        } else {
            is_excess.push_back(false);
            surplus_types.push_back(k);
            total_surplus -= difference.back();
        }
    }
    display(demand, "Demand");
    display(cost, "Cost");
    cout << "gamma = " << gamma << endl;
    display(capacity, "Capacity");
    display(Capacity_by_k, "Capacity_by_k");
    cout << "Data input passes." << endl;

    //gamma = 1;

    //cout << "Excess_tsp = " << excess_tsp() << endl;
    
    best_surplus = 0;
    //cout << "Best cost = " << best_surplus << endl;
    //best_curve.display();

    cout << "Obj = " << excess_tsp() + surplus_tsp() + direct_misplace << endl;
    return 0;
}

double excess_tsp(){
    vector<double> V_E (1<<excess_types.size(), 0);
    vector<double> D (1<<excess_types.size(), 0);
    D[0] = direct_misplace;
    for (int i = 1; i < V_E.size() - 1; i++){
        for (int j = 0; j < excess_types.size(); j++){
            if ((1<<j) & i) { //if the j+1 position of i is 1
                int k = excess_types[j];
                if (D[i] == 0) {
                    D[i] = D[i ^ (1<<j)] - difference[k];
                }
                if (V_E[i] < min(D[i], Capacity_by_k[k]) + V_E[i ^ (1<<j)]) {
                    V_E[i] = min(D[i], Capacity_by_k[k]) + V_E[i ^ (1<<j)];
                }
            }
        }
    }
    int i = V_E.size()-1;
    for (int j = 0; j < excess_types.size(); j++){
        if (V_E[i] < V_E[i ^ (1<<j)]){
            V_E[i] = V_E[i ^ (1<<j)];
        }
    }
    return V_E.back();
}

double surplus_tsp(){
    vector<double> V_S (1<<surplus_types.size(), 0);
    vector<double> surplus_S (1<<surplus_types.size(), 0);
    for (int i = 1; i < V_S.size(); i++){
        double D = 0;
        for (int j = 0; j < surplus_types.size(); j++) {
            if ((1<<j) & i) {
                int k = surplus_types[j];
                if (surplus_S[i] == 0) {
                    surplus_S[i] = surplus_S[i ^ (1<<j)] - difference[k];
                    D = max(direct_misplace - (total_surplus - surplus_S[i]), 0.0);
                }
                if (V_S[i] < min(max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)]) {
                    V_S[i] = min(max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)];
                }
            }
        }
    }
    return V_S.back();
}

double worst_assignment(){
    
}