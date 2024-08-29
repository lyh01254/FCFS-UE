#include "myIOS.h"
#include "common.h"
#include "curve.h"
#include<chrono>
#include<fstream>
#include<numeric>
#include<algorithm>

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
double total_surplus;
vector<double> difference;
Curve best_curve;
double best_surplus;

string root = "../data/totally_random/";
string output = "../results/algo_detail.csv";
string ave_output = "../results/algo_ave.csv";
string log_output = "../results/algo_log.txt";

double excess_tsp();
double surplus_tsp(vector<double>& worst_assign_by_type);
void surplus_solve();
void enumerate(vector<int>& sequence, int start, unordered_map<string, Curve>& V_S, const vector<Curve>& v);
void solve(const vector<int>& sequence, unordered_map<string, Curve>& V_S, const vector<Curve>& v);
void light_solve(const vector<int>& sequence, const vector<Curve>& v);
void light_enumerate(vector<int>& sequence, int start, const vector<Curve>& v);
void light_surplus_solve();

int main(int argc, char* argv[]){
    ofstream file;
    file.open(output, ios::out|ios::trunc);
    file << "I,K,instance, so_assignment, so_misplace, so_obj, best_assign, worst_assign, best_misplace, worst_misplace, best, worst, time_best, time_worst, worst_lb, worst_ub, gap, time_bound" << endl;
    file.close();
    ofstream ave_file;
    ave_file.open(ave_output, ios::out|ios::trunc);
    ave_file << "I,K,best_min,best_max,best_ave,worst_min,worst_max,worst_ave, bound_ave, gap_ave" << endl;
    ave_file.close();
    ofstream log_file;
    log_file.open(log_output, ios::out|ios::trunc);
    log_file << "LOG" << endl;
    log_file.close();
    for (NbHotels = 20; NbHotels <= 100; NbHotels += 20){
        for (NbTypes = 4; NbTypes <= 16; NbTypes += 2){
            double best_min = 999999999;
            double worst_min = 999999999;
            double best_max = 0;
            double worst_max = 0;
            double best_ave, worst_ave, gap_ave, bound_ave;
            for (int instance = 1; instance <= 5; instance++){
                log_file.open(log_output, ios::out|ios::app);
                log_file << "Start to solve" << to_string(NbHotels) + "_" + to_string(NbTypes) + "_" + to_string(instance) << endl;
                log_file.close();
                string file_name = root+to_string(NbHotels) + "_" + to_string(NbTypes) + "_" + to_string(instance) + ".csv"; 
                demand.clear();
                capacity.clear();
                cost.clear();
                Capacity_by_k.clear();
                is_excess.clear();
                excess_types.clear();
                surplus_type.clear();
                difference.clear();
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

                double so_assignment = 0;
                double so_misplace = 0;
                double so_obj = 0;
                double best_assign = 0;
                double worst_assign = 0;
                double best_misplace = 0;
                double worst_misplace = 0;
                double best = 0;
                double worst = 0;
                double time_best = 0;
                double time_worst = 0;
                double excess_tsp_value = 0;
                double worst_ub = 0;
                double worst_lb = 0;
                double time_bound = 0;

                //* obtain the best-case UE result
                auto start = chrono::high_resolution_clock::now();
                vector<double> Q_k(demand);
                vector<vector<double> > C(capacity);
                vector<int> cost_index(NbHotels);
                vector<vector<vector<double> > > assign(NbHotels, vector<vector<double> > (NbTypes, vector<double> (NbTypes, 0)));
                iota(cost_index.begin(), cost_index.end(), 0);
                sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] < cost[j];});
                double sum_Q = accumulate(Q_k.begin(), Q_k.end(), 0);
                auto it1 = cost_index.begin();
                auto it2 = cost_index.begin();
                while (sum_Q > 0){
                    if (it1 != cost_index.end()){
                        //* *it1 = i_star
                        for (int w = 0; w < NbTypes; w++){
                            if (C[*it1][w] > 0){
                                assign[*it1][w][w] = min(Q_k[w], C[*it1][w]);
                                C[*it1][w] -= assign[*it1][w][w];
                                Q_k[w] -= assign[*it1][w][w];
                                sum_Q -= assign[*it1][w][w];
                                best_assign += assign[*it1][w][w] * cost[*it1];
                            }
                        }
                        it1++;
                    } else {
                        //* *it2 = i_star
                        for (int w = 0; w < NbTypes; w++){
                            if (C[*it2][w] > 0){
                                for (int k = 0; k < NbTypes; k++){
                                    if (k != w && Q_k[k] > 0 && C[*it2][w] > 0){
                                        assign[*it2][k][w] = min(Q_k[k], C[*it2][w]);
                                        C[*it2][w] -= assign[*it2][k][w];
                                        Q_k[k] -= assign[*it2][k][w];
                                        sum_Q -= assign[*it2][k][w];
                                        best_assign += assign[*it2][k][w] * cost[*it2];
                                        best_misplace += assign[*it2][k][w];
                                    }
                                }
                            }
                        }
                        it2++;
                    }
                }
                best = best_assign + best_misplace * gamma;
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double, std::nano> elapsed = end - start;
                time_best = elapsed.count();
                //* at the same time we get the best assignment and misplace under UE

                //* obtain the worst assignment under UE
                Q_k = demand;
                C = capacity;
                assign = vector<vector<vector<double>>>(NbHotels, vector<vector<double> > (NbTypes, vector<double> (NbTypes, 0)));
                sum_Q = accumulate(Q_k.begin(), Q_k.end(), 0);
                sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] > cost[j];});
                it1 = cost_index.begin();
                it2 = cost_index.begin();
                while (sum_Q > 0){
                    if (it1 != cost_index.end()){
                        //* *it1 = i_star
                        for (int w = 0; w < NbTypes; w++){
                            if (C[*it1][w] > 0){
                                assign[*it1][w][w] = min(Q_k[w], C[*it1][w]);
                                C[*it1][w] -= assign[*it1][w][w];
                                Q_k[w] -= assign[*it1][w][w];
                                sum_Q -= assign[*it1][w][w];
                                worst_assign += assign[*it1][w][w] * cost[*it1];
                            }
                        }
                        it1++;
                    } else {
                        //* *it2 = i_star
                        for (int w = 0; w < NbTypes; w++){
                            if (C[*it2][w] > 0){
                                for (int k = 0; k < NbTypes; k++){
                                    if (k != w && Q_k[k] > 0 && C[*it2][w] > 0){
                                        assign[*it2][k][w] = min(Q_k[k], C[*it2][w]);
                                        C[*it2][w] -= assign[*it2][k][w];
                                        Q_k[k] -= assign[*it2][k][w];
                                        sum_Q -= assign[*it2][k][w];
                                        worst_assign += assign[*it2][k][w] * cost[*it2];
                                    }
                                }
                            }
                        }
                        it2++;
                    }
                }

                //* obtain the worst obj under UE, timing
                start = chrono::high_resolution_clock::now();
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
                        surplus_type.push_back(k);
                        total_surplus -= difference.back();
                    }
                }
                best_surplus = 0;
                light_surplus_solve();
                excess_tsp_value = excess_tsp();
                worst = best_surplus + excess_tsp_value * gamma + excess_assignment_cost;
                end = chrono::high_resolution_clock::now();
                elapsed = end - start;
                time_worst = elapsed.count();

                //* obtain the worst misplace under UE
                start = chrono::high_resolution_clock::now();
                vector<double> worst_assign_by_type(NbTypes, 0);
                for (int k = 0; k < NbTypes; k++){
                    if (is_excess[k]){
                        worst_assign_by_type[k] = Capacity_by_k[k];
                    }
                }
                worst_misplace = excess_tsp() + surplus_tsp(worst_assign_by_type) + direct_misplace;
                worst_ub = worst_assign + worst_misplace * gamma;
                double worst_assign_con = 0;
                for (int k = 0; k < NbTypes; k++){
                    if (worst_assign_by_type[k] == Capacity_by_k[k]){
                        for (int i = 0; i < NbHotels; i++){
                            worst_assign_con += capacity[i][k] * cost[i];
                        }
                    } else if (worst_assign_by_type[k] < Capacity_by_k[k]){
                        auto it3 = cost_index.begin();
                        double a_k = worst_assign_by_type[k];
                        while (a_k > 0) {
                            worst_assign_con += min(a_k, capacity[*it3][k]) * cost[*it3];
                            a_k -= min(a_k, capacity[*it3][k]);
                            it3++;
                        }
                    }
                }
                worst_lb = worst_misplace * gamma + worst_assign_con;
                end = chrono::high_resolution_clock::now();
                elapsed = end - start;
                time_bound = elapsed.count();

                double gap = (worst_ub - worst_lb)/worst;

                file.open(output, ios::out|ios::app);
                file << NbHotels << "," << NbTypes << "," << instance << "," << so_assignment << "," << so_misplace << "," << so_obj << "," << best_assign << "," << worst_assign << "," << best_misplace << "," << worst_misplace << "," << best << "," << worst << "," << time_best << "," << time_worst << "," << worst_lb << "," << worst_ub << "," << gap << "," << time_bound << endl;
                file.close();
                if (best_min > time_best){
                    best_min = time_best;
                }
                if (best_max < time_best){
                    best_max = time_best;
                }
                if (worst_min > time_worst){
                    worst_min = time_worst;
                }
                if (worst_max < time_worst){
                    worst_max = time_worst;
                }
                best_ave = (best_ave * (instance - 1) + time_best) / instance;
                worst_ave = (worst_ave * (instance - 1) + time_worst) / instance;
                gap_ave = (gap_ave * (instance - 1) + gap) / instance;  
                bound_ave = (bound_ave * (instance - 1) + time_bound) / instance;  
            }
            ave_file.open(ave_output, ios::out|ios::app);
            ave_file << NbHotels << "," << NbTypes << "," << best_min << "," << best_max << "," << best_ave << "," << worst_min << "," << worst_max << "," << worst_ave << "," << bound_ave << "," << gap_ave << endl;
            ave_file.close();
        }
    }
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
        //display(sequence, "sequence to be checked:");
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
    //cout << "key = " << key << endl;
    //cout << "checked V_S" << endl;
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

double surplus_tsp(vector<double>& worst_assign_by_type){
    vector<double> V_S (1<<surplus_type.size(), 0);
    vector<double> surplus_S (1<<surplus_type.size(), 0);
    for (int i = 1; i < V_S.size(); i++){
        double D = 0;
        for (int j = 0; j < surplus_type.size(); j++) {
            if ((1<<j) & i) {
                int k = surplus_type[j];
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
    for (int i = V_S.size()-1; i > 0; ){
        for (int j = 0; j < surplus_type.size(); j++){
            if ((1<<j) & i) {
                int k = surplus_type[j];
                double D = max(direct_misplace - (total_surplus - surplus_S[i]), 0.0);
                if (V_S[i] == min(max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)]){
                    if (D + difference[k] > 0) {
                        worst_assign_by_type[k] = Capacity_by_k[k];
                    } else {
                        worst_assign_by_type[k] = demand[k] + D;
                    } 
                    i = (i ^ (1<<j));
                }  
            }
        }
    }
    return V_S.back();
}

void light_solve(const vector<int>& sequence, const vector<Curve>& v){
    auto it = sequence.end()-1;
    Curve V;
    v2V(V, v[surplus_type[*it]]);
    while (--it >= sequence.begin()){
        int k = surplus_type[*it];
        Curve raised_V;
        raise(raised_V, V, v[k], gamma);
        //cout << "raised" << endl;
        vector<Curve> IC_curve(V.get_NbCons());
        inf_convolute(IC_curve, V, v[k]);
        //cout << "IC" << endl;
        Curve new_V;
        join(new_V, raised_V, IC_curve);
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
        //display(sequence, "sequence to be checked:");
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