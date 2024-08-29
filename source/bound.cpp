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

string root = "data/totally_random/";
string output = "results/bound.csv";
string ave_output = "results/bound_ave.csv";
string log_output = "results/bound_log.txt";

double excess_tsp();
double surplus_tsp();
double excess_bound();
double surplus_bound();

int main(int argc, char* argv[]){
    ofstream file;
    file.open(output, ios::out|ios::trunc);
    file << "I,K,id,gamma, misplace_lb,misplace_ub,misplace,gap,bound_sort,true_sort,i_misplace,i_ub,i_gap,i_percent,i_b_sort,i_true_sort,misplace_lb1,misplace_ub1,misplace1,gap1,bound_sort1,true_sort1,i_misplace1,i_ub1,i_gap1,i_percent1,i_b_sort1,i_true_sort1,misplace_lb2,misplace_ub2,misplace2,gap2,bound_sort2,true_sort2,i_misplace2,i_ub2,i_gap2,i_percent2,i_b_sort2,i_true_sort2" << endl;
    file.close();
    ofstream ave_file;
    ave_file.open(ave_output, ios::out|ios::trunc);
    ave_file << "I,K,gap,accurate,i_gap,i_acc,i_percent,gap1,accurate1,i_gap1,i_acc1,i_percent1,gap2,accurate2,i_gap2,i_acc2,i_percent2" << endl;
    ave_file.close();
    ofstream log_file;
    log_file.open(log_output, ios::out|ios::trunc);
    log_file << "LOG" << endl;
    log_file.close();
    for (NbHotels = 20; NbHotels <= 100; NbHotels += 20){
        for (NbTypes = 4; NbTypes <= 16; NbTypes += 2){
            std::vector<double> gap_ave(3, 0);
            std::vector<double> improved_gap_ave(3, 0);
            std::vector<double> accuracy(3, 0);
            std::vector<double> improvement_ave(3, 0);
            std::vector<double> improved_accuracy(3, 0);
            std::vector<std::vector<double>> misplace_lb(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> misplace_ub(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> misplace(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> gap(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> bound_sort(3, std::vector<double>({0,1,2,3,4}));
            std::vector<std::vector<double>> true_sort(3, std::vector<double>({0,1,2,3,4}));
            std::vector<std::vector<double>> improved_misplace(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> improved_ub(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> improved_gap(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> improvement(3, std::vector<double>(5, 0));
            std::vector<std::vector<double>> improved_sort(3, std::vector<double>({0,1,2,3,4}));
            std::vector<std::vector<double>> improved_true_sort(3, std::vector<double>({0,1,2,3,4}));
            vector<double> gamma_i(5,0);
            for (int instance = 0; instance < 5; instance++){
                log_file.open(log_output, ios::out|ios::app);
                log_file << "Start to solve" << to_string(NbHotels) + "_" + to_string(NbTypes) + "_" + to_string(instance+1) << endl;
                log_file.close();
                string file_name = root+to_string(NbHotels) + "_" + to_string(NbTypes) + "_" + to_string(instance+1) + ".csv"; 
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
                gamma_i[instance] = gamma;

                //* 
                direct_misplace = 0;
                total_surplus = 0;
                
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
                    } else {
                        is_excess.push_back(false);
                        surplus_type.push_back(k);
                        total_surplus -= difference.back();
                    }
                }
                misplace_lb[0][instance] = direct_misplace;
                misplace_lb[1][instance] = direct_misplace;
                misplace_lb[2][instance] = 0;
                
                //todo: compute 1 for leading
                misplace[1][instance] = excess_tsp();
                misplace_ub[1][instance] = excess_bound();
                gap[1][instance] = (misplace_ub[1][instance] - misplace[1][instance]) / misplace[1][instance];

                //todo: compute 2 for following
                misplace[2][instance] = surplus_tsp()+direct_misplace;
                misplace_ub[2][instance] = surplus_bound()+direct_misplace;
                gap[2][instance] = (misplace_ub[2][instance] - misplace[2][instance]) / misplace[2][instance];

                //todo: combine 0 by combining 1 & 2 
                misplace[0][instance] = misplace[1][instance] + misplace[2][instance];
                misplace_ub[0][instance] = misplace_ub[1][instance] + misplace_ub[2][instance];
                gap[0][instance] = (misplace_ub[0][instance] - misplace[0][instance]) / misplace[0][instance];

                vector<double> tentative_allocation(NbTypes, 0);
                //todo: compute the improved case for 1
                double a1 = 0;
                vector<int> sorted_index_E(excess_types);
                sort(sorted_index_E.begin(), sorted_index_E.end(), [&](int i, int j){return demand[i] < demand[j];});
                double remain_excess = direct_misplace;
                int remain_types = excess_types.size();
                double sum_C = 0;
                for (int i = 0; i < excess_types.size(); i++){
                    tentative_allocation[sorted_index_E[i]] = min(demand[sorted_index_E[i]], remain_excess/remain_types);
                    a1 += i * tentative_allocation[sorted_index_E[i]];
                    remain_excess -= tentative_allocation[sorted_index_E[i]];
                    remain_types--;
                    sum_C += Capacity_by_k[sorted_index_E[i]];
                }
                double b1 = sum_C - sum_C/excess_types.size();
                if (a1 < b1) {
                    for (int i = 0; i < excess_types.size(); i++){
                        int k = excess_types[i];
                        difference[k] = tentative_allocation[k];
                        Capacity_by_k[k] = demand[k] - tentative_allocation[k];
                    }
                } else {
                    for (int i = 0; i < excess_types.size(); i++){
                        int k = excess_types[i];
                        Capacity_by_k[k] = sum_C/excess_types.size();
                        difference[k] = demand[k] - Capacity_by_k[k];
                    }
                }
                improved_misplace[1][instance] = excess_tsp();
                improved_ub[1][instance] = excess_bound();
                improved_gap[1][instance] = (improved_ub[1][instance] - improved_misplace[1][instance]) / improved_misplace[1][instance];
                improvement[1][instance] = (misplace[1][instance] - improved_misplace[1][instance]) / misplace[1][instance];

                //todo: compute the improved case for 2
                for (int i = 0; i < surplus_type.size(); i++){
                    int k = surplus_type[i];
                    tentative_allocation[k] = -total_surplus/surplus_type.size();
                    difference[k] = tentative_allocation[k];
                    Capacity_by_k[k] = demand[k] - difference[k];
                }
                improved_misplace[2][instance] = surplus_tsp()+direct_misplace;
                improved_ub[2][instance] = surplus_bound()+direct_misplace;
                improved_gap[2][instance] = (improved_ub[2][instance] - improved_misplace[2][instance]) / improved_misplace[2][instance];
                improvement[2][instance] = (misplace[2][instance] - improved_misplace[2][instance]) / misplace[2][instance];

                //todo: compute the improved case for 0 combining 1 & 2
                improved_misplace[0][instance] = improved_misplace[1][instance] + improved_misplace[2][instance];
                improved_ub[0][instance] = improved_ub[1][instance] + improved_ub[2][instance];
                improved_gap[0][instance] = (improved_ub[0][instance] - improved_misplace[0][instance]) / improved_misplace[0][instance];
                improvement[0][instance] = (misplace[0][instance] - improved_misplace[0][instance]) / misplace[0][instance];
                //cout << "I = " << NbHotels << " K = " << NbTypes << " id = " << instance << " misplace = " << misplace[0][instance] << " i_misplace = " << improved_misplace[0][instance] << " improve = " << improvement[0][instance] << endl;

            }
            //todo: compute sort and ave
            for (int i = 0; i < 3; i++) {
                sort(bound_sort[i].begin(),bound_sort[i].end(),[&](int k, int j){return misplace_ub[i][k] < misplace_ub[i][j];});
                sort(true_sort[i].begin(),true_sort[i].end(),[&](int k, int j){return misplace[i][k] < misplace[i][j];}); 
                sort(improved_sort[i].begin(),improved_sort[i].end(),[&](int k, int j){return improved_ub[i][k] < improved_ub[i][j];});  
                sort(improved_true_sort[i].begin(),improved_true_sort[i].end(),[&](int k, int j){return improved_misplace[i][k] < improved_misplace[i][j];});
            }
            for (int instance = 0; instance < 5; instance++) {
                file.open(output, ios::out|ios::app);
                file << NbHotels << "," << NbTypes << "," << instance << "," << gamma_i[instance] << ",";
                for (int i = 0; i < 3; i++) {
                    file << misplace_lb[i][instance] << "," << misplace_ub[i][instance] << "," << misplace[i][instance] << "," << gap[i][instance] << "," << bound_sort[i][instance] << "," << true_sort[i][instance] << ",";
                    file << improved_misplace[i][instance] << "," << improved_ub[i][instance] << "," << improved_gap[i][instance] << "," << improvement[i][instance] << "," << improved_sort[i][instance] << "," << improved_true_sort[i][instance] << ",";
                }
                file << endl;
                file.close();
            }
            for (int i = 0; i < 3; i++){
                for (int instance = 0; instance < 5; instance++){
                    accuracy[i] += (bound_sort[i][instance] == true_sort[i][instance]);
                    improved_accuracy[i] += (improved_sort[i][instance] == improved_true_sort[i][instance]);
                }
                gap_ave[i] = accumulate(gap[i].begin(), gap[i].end(), 0.0) / 5;
                improved_gap_ave[i] = accumulate(improved_gap[i].begin(), improved_gap[i].end(), 0.0) / 5;
                improvement_ave[i] = accumulate(improvement[i].begin(), improvement[i].end(), 0.0) / 5;

            }
            cout << "I = " << NbHotels << " K = " << NbTypes << " ";
            display(improvement[0]);
            ave_file.open(ave_output, ios::out|ios::app);
            ave_file << NbHotels << "," << NbTypes << ",";
            for (int i = 0; i < 3; i++){
                ave_file << gap_ave[i] << "," << accuracy[i] << "," << improved_gap_ave[i] << "," << improved_accuracy[i] << "," << improvement_ave[i] << ",";
            }
            ave_file << endl;
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


double surplus_tsp(){
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
    return V_S.back();
}

double excess_bound(){
    vector<int> sorted_index(excess_types);
    sort(sorted_index.begin(),sorted_index.end(),[&](int i, int j){return difference[i] >= difference[j];});
    double a = 0;
    double sum_C = 0;
    double min_C = 999999999;
    for (int i = 0; i < excess_types.size(); i++){
        int t = i+1;
        a += (excess_types.size()-t) * difference[sorted_index[i]];
        sum_C += Capacity_by_k[sorted_index[i]];
        if (min_C > Capacity_by_k[sorted_index[i]]){
            min_C = Capacity_by_k[sorted_index[i]];
        }
    }
    double b = sum_C - min_C;
    return min(a,b);
}

double surplus_bound(){
    vector<int> sorted_index(surplus_type);
    sort(sorted_index.begin(),sorted_index.end(),[&](int i, int j){return difference[i] >= difference[j];});
    double a = 0;
    double sum_Q = 0;
    double min_Q = 999999999;
    double sum_delta = 0;
    for (int i = 0; i < surplus_type.size(); i++){
        sum_delta += -difference[sorted_index[i]];
        if (direct_misplace > sum_delta) {
            a += direct_misplace - sum_delta;
        }
        sum_Q += demand[sorted_index[i]];
        if (min_Q > demand[sorted_index[i]]){
            min_Q = demand[sorted_index[i]];
        }
    }
    double b = sum_Q - min_Q;
    return min(a,b);
}