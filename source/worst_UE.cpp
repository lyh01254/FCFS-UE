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
vector<int> excess_types;
vector<int> surplus_type;
double direct_misplace;
vector<double> difference;
Curve best_curve;
double best_surplus;


string root = "data/totally_random/";

double excess_tsp();
void surplus_solve();
void enumerate(vector<int>& sequence, int start, unordered_map<string, Curve>& V_S, const vector<Curve>& v);
void solve(const vector<int>& sequence, unordered_map<string, Curve>& V_S, const vector<Curve>& v);
void light_solve(const vector<int>& sequence, const vector<Curve>& v);
void light_enumerate(vector<int>& sequence, int start, const vector<Curve>& v);
void light_surplus_solve();

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
            surplus_type.push_back(k);
        }
    }
    display(demand, "Demand");
    display(cost, "Cost");
    cout << "gamma = " << gamma << endl;
    display(capacity, "Capacity");
    display(Capacity_by_k, "Capacity_by_k");
    cout << "NbExcess = " << excess_types.size() << endl;
    cout << "NbSurplus = " << surplus_type.size() << endl;
    cout << "Data input passes." << endl;

    //gamma = 1;

    //cout << "Excess_tsp = " << excess_tsp() << endl;
    
    best_surplus = 0;
    light_surplus_solve();
    //cout << "Best cost = " << best_surplus << endl;
    //best_curve.display();
    cout << "best_surplus = " << best_surplus << endl;
    cout << "excess_tsp = " << excess_tsp() << endl;
    cout << "excess_assignment_cost = " << excess_assignment_cost << endl;

    cout << "Obj = " << (best_surplus+excess_tsp()*gamma+excess_assignment_cost) << endl;
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

void surplus_solve(){
    unordered_map<string, Curve> V_S;
    vector<Curve> v(NbTypes);
    find_v(v,demand, capacity, cost, gamma);
    for (int i = 0; i < surplus_type.size(); i++){
        int k = surplus_type[i];
        V_S.emplace(to_string(i), Curve());
        v2V(V_S[to_string(i)], v[k]);
    }
    vector<int> sequence(surplus_type.size());
    iota(sequence.begin(), sequence.end(), 0);
    enumerate(sequence, 0, V_S, v);
}

void enumerate(vector<int>& sequence, int start, unordered_map<string, Curve>& V_S, const vector<Curve>& v){
    if (start == sequence.size()-1) {
        display(sequence, "sequence to be checked:");
        solve(sequence, V_S, v);
    } else {
        int original_start = sequence[start];
        for (int i = start; i < sequence.size(); i++){
            int temp = sequence[i];
            sequence[i] = original_start;
            sequence[start] = temp;
            enumerate(sequence, start+1, V_S, v);
            sequence[i] = temp;
            sequence[start] = original_start;
        }
    }
}

void solve(const vector<int>& sequence, unordered_map<string, Curve>& V_S, const vector<Curve>& v){
    auto it = sequence.end()-1;
    string key = to_string(*it);
    Curve* ptr;
    while (V_S.count(key)){
        ptr = &V_S[key];
        it--;
        key = to_string(*it) + "-" + key;
    }
    ptr->display();
    while (true){
        int k = surplus_type[*it];
        Curve raised_V;
        raise(raised_V, *ptr, v[k], gamma);
        //cout << "raised" << endl;
        vector<Curve> IC_curve(ptr->get_NbCons());
        inf_convolute(IC_curve, *ptr, v[k]);
        //cout << "IC" << endl;
        V_S.emplace(key, Curve());
        join(V_S[key], raised_V, IC_curve);
        //cout << "joined" << endl;
        if (--it >= sequence.begin()){
            ptr = &V_S[key];
            key = to_string(*it) + "-" + key;
        } else {
            break;
        }
    }
    if (V_S[key].value_at(direct_misplace) > best_surplus) {
        best_surplus = V_S[key].value_at(direct_misplace);
        best_curve = V_S[key];
    }
}

void light_solve(const vector<int>& sequence, const vector<Curve>& v){
    auto it = sequence.end()-1;
    Curve V;
    v2V(V, v[surplus_type[*it]]);
    //V.display();

    while (--it >= sequence.begin()){
        int k = surplus_type[*it];
        Curve raised_V;
        raise(raised_V, V, v[k], gamma);
        //raised_V.display();
        //cout << "raised" << endl;
        vector<Curve> IC_curve(V.get_NbCons());
        inf_convolute(IC_curve, V, v[k]);
        //cout << "IC" << endl;
        Curve new_V;
        join(new_V, raised_V, IC_curve);
        //new_V.display();
        //cout << "joined" << endl;
        V = new_V;
    }
    if (V.value_at(direct_misplace) > best_surplus) {
        best_surplus = V.value_at(direct_misplace);
        best_curve = V;
    }
}

void light_enumerate(vector<int>& sequence, int start, const vector<Curve>& v){
    if (start == sequence.size()-1) {
        display(sequence, "sequence to be checked:");
        light_solve(sequence, v);
    } else {
        int original_start = sequence[start];
        for (int i = start; i < sequence.size(); i++){
            int temp = sequence[i];
            sequence[i] = original_start;
            sequence[start] = temp;
            light_enumerate(sequence, start+1, v);
            sequence[i] = temp;
            sequence[start] = original_start;
        }
    }
}

void light_surplus_solve(){
    vector<Curve> v(NbTypes);
    find_v(v,demand, capacity, cost, gamma);
    vector<int> sequence(surplus_type.size());
    iota(sequence.begin(), sequence.end(), 0);
    light_enumerate(sequence, 0, v);
}