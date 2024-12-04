#include<iostream>
#include<vector>
#include<random>
#include<fstream>
#include"common.h"

int NbHotels, NbTypes, gamma;
void generate_cost(std::vector<int>& cost, const double& lb, const double& ub);
int generate_gamma(const std::vector<int>& cost);
void generate_capacity(std::vector<std::vector<int> >& capacity, const int& min_K, const double& lb, const double& ub);
void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const double& excess_rate, const double& surplus_rate);
void write(const std::string& folder, const std::string& filename, const std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const std::vector<int> cost, const int& gamma);
void generate_long(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const int& NbSurplus, const double& excess_rate, const double& surplus_rate);
void generate_cost_variance();
void generate_excess_variance();
void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const double& excess_lb, const double& excess_ub, const double& surplus_lb, const double& surplus_ub);

int main(){
    generate_excess_variance();
    return 0;
}

void normal_generate(){
    double MIN_COST = 1;
    double MAX_COST = 50;
    double MIN_CAPACITY = 10;
    double MAX_CAPACITY = 100;
    double EXCESS_RATE = 0.5;
    double SURPLUS_RATE = 0.5;
    double EXCESS_PROB = 0.4;
    for (NbHotels = 40; NbHotels <= 100; NbHotels+=20){
        for (NbTypes = 16; NbTypes <= 16; NbTypes+=2){
            for (int NbSurplus = 9; NbSurplus <= 10; NbSurplus++){
                for (int instance = 1; instance <= 5; instance++){
                    int MIN_K = std::max(NbTypes/2, 4);
                    std::vector<int> cost(NbHotels, 0);
                    std::vector<int> demand(NbTypes, 0);
                    std::vector<std::vector<int> > capacity(NbHotels, std::vector<int>(NbTypes, 0));
                    generate_cost(cost, MIN_COST, MAX_COST);
                    int gamma = generate_gamma(cost);
                    generate_capacity(capacity, MIN_K, MIN_CAPACITY, MAX_CAPACITY);
                    //generate_demand(demand, capacity, EXCESS_PROB, EXCESS_RATE, SURPLUS_RATE);
                    generate_long(demand, capacity, NbSurplus, EXCESS_RATE, SURPLUS_RATE);
                    //std::string filename = std::to_string(NbHotels) + "_" + std::to_string(NbTypes) + "_" + std::to_string(instance) + ".csv";
                    std::string filename = std::to_string(NbHotels) + "_" + std::to_string(NbTypes) + "_" + std::to_string(NbSurplus) + "_" + std::to_string(instance) + ".csv";
                    std::string folder = "data/long_surplus/";
                    write(folder, filename, demand, capacity, cost, gamma);
                }
            }
        }
    }
}

void generate_excess_variance(){
    double MIN_CAPACITY = 10;
    double MAX_CAPACITY = 100;
    double MIN_COST = 1;
    double MAX_COST = 50;
    double SURPLUS_RATE = 0.5;
    NbHotels = 20;
    NbTypes = 8;
    int MIN_K = std::max(NbTypes/2, 4);
    std::vector<int> cost(NbHotels, 0);
    std::vector<int> demand(NbTypes, 0);
    std::vector<std::vector<int> > capacity(NbHotels, std::vector<int>(NbTypes, 0));
    generate_capacity(capacity, MIN_K, MIN_CAPACITY, MAX_CAPACITY);
    generate_cost(cost, MIN_COST, MAX_COST);
    int gamma = generate_gamma(cost);
    for (int rate = 3; rate <= 5; rate++){
        double EXCESS_RATE = rate * 0.1;
        for (int instance = 1; instance <= 10; instance++){
            generate_demand(demand, capacity, 0.5-EXCESS_RATE, EXCESS_RATE, 0.5-SURPLUS_RATE, SURPLUS_RATE);
            std::string filename = std::to_string(NbHotels) + "_" + std::to_string(NbTypes) + "_" + std::to_string(rate) + "_" + std::to_string(instance) + ".csv";
            std::string folder = "data/excess2/";
            write(folder, filename, demand, capacity, cost, gamma);
        }
    }
}

void generate_cost(std::vector<int>& cost, const double& lb, const double& ub){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(lb,ub);
    for (auto it = cost.begin(); it != cost.end(); it++){
        *it = dis(gen);
    }
}

int generate_gamma(const std::vector<int>& cost){
    int min_diff = 99999999;
    int max_diff = -99999999;
    for (int i = 0; i < NbHotels; i++){
        for (int j = 0; j < NbHotels; j++){
            if (j != i){
                int diff = abs(cost[i] - cost[j]);
                if (diff < min_diff) {
                    min_diff = diff;
                }
                if (diff > max_diff){
                    max_diff = diff;
                }
            }
        }
    }

    int lb = min_diff;
    int ub = max_diff + (max_diff - min_diff) / 2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(lb,ub);
    return dis(gen);
}

void generate_capacity(std::vector<std::vector<int> >& capacity, const int& min_K, const double& lb, const double& ub){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_K(min_K, NbTypes);
    std::uniform_int_distribution<> dis_C(lb, ub);
    std::uniform_real_distribution<> dis(0,1);
    for (int k = 0; k < NbTypes; k++){
        double prob = dis_K(gen) / (NbTypes * 1.0);
        //std::cout << "prob = " << prob << std::endl;
        for (int i = 0; i < NbHotels; i++){
            if (dis(gen) < prob) {
                capacity[i][k] = dis_C(gen);
            }
        }
    }
}

void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const double& excess_rate, const double& surplus_rate) {
    int sum_C = 0;
    std::vector<int> C_k(NbTypes, 0);
    for (int k = 0; k < NbTypes; k++){
        for (int i = 0; i < NbHotels; i++){
            sum_C += capacity[i][k];
            C_k[k] += capacity[i][k];
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);
    std::uniform_int_distribution<> dis_0(C_k[0], (1+excess_rate)*C_k[0]);
    while (true){
        int sum_Q = 0;
        for (int k = 0; k < NbTypes - 1; k++){
            if (k < NbTypes/2.0) {
                std::uniform_int_distribution<> dis_C(C_k[k], (1+excess_rate)*C_k[k]);
                demand[k] = dis_C(gen);
            } else {
                std::uniform_int_distribution<> dis_C((1-surplus_rate)*C_k[k], C_k[k]);
                demand[k] = dis_C(gen);
            }
            sum_Q += demand[k];
        }
        if (sum_C - sum_Q > 1) {
            int ub;
            if (sum_C - sum_Q < C_k[NbTypes-1]){
                ub = sum_C - sum_Q;
            } else {
                ub = C_k[NbTypes-1];
            }
            std::uniform_int_distribution<> dis_C(1, ub);
            demand[NbTypes-1] = dis_C(gen);
            break;
        }
    }
}

void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const double& excess_lb, const double& excess_ub, const double& surplus_lb, const double& surplus_ub) {
    int sum_C = 0;
    std::vector<int> C_k(NbTypes, 0);
    for (int k = 0; k < NbTypes; k++){
        for (int i = 0; i < NbHotels; i++){
            sum_C += capacity[i][k];
            C_k[k] += capacity[i][k];
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);
    while (true){
        int sum_Q = 0;
        for (int k = 0; k < NbTypes - 1; k++){
            if (k < NbTypes/2.0) {
                std::uniform_int_distribution<> dis_C((1+excess_lb)*C_k[k], (1+excess_ub)*C_k[k]);
                demand[k] = dis_C(gen);
            } else {
                std::uniform_int_distribution<> dis_C((1-surplus_ub)*C_k[k], (1-surplus_lb)*C_k[k]);
                demand[k] = dis_C(gen);
            }
            sum_Q += demand[k];
        }
        if (sum_C - sum_Q > 1) {
            int ub;
            if (sum_C - sum_Q < C_k[NbTypes-1]){
                ub = sum_C - sum_Q;
            } else {
                ub = C_k[NbTypes-1];
            }
            std::uniform_int_distribution<> dis_C(1, ub);
            demand[NbTypes-1] = dis_C(gen);
            break;
        }
    }
}

void generate_long(std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const int& NbSurplus, const double& excess_rate, const double& surplus_rate) {
    int sum_C = 0;
    std::vector<int> C_k(NbTypes, 0);
    for (int k = 0; k < NbTypes; k++){
        for (int i = 0; i < NbHotels; i++){
            sum_C += capacity[i][k];
            C_k[k] += capacity[i][k];
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);
    std::uniform_int_distribution<> dis_0(C_k[0], (1+excess_rate)*C_k[0]);
    while (true){
        int sum_Q = 0;
        for (int k = 0; k < NbTypes - 1; k++){
            if (k < NbTypes - NbSurplus) {
                std::uniform_int_distribution<> dis_C(C_k[k], (1+excess_rate)*C_k[k]);
                demand[k] = dis_C(gen);
            } else {
                std::uniform_int_distribution<> dis_C((1-surplus_rate)*C_k[k], C_k[k]);
                demand[k] = dis_C(gen);
            }
            sum_Q += demand[k];
        }
        if (sum_C - sum_Q > 1) {
            int ub;
            if (sum_C - sum_Q < C_k[NbTypes-1]){
                ub = sum_C - sum_Q;
            } else {
                ub = C_k[NbTypes-1];
            }
            std::uniform_int_distribution<> dis_C(1, ub);
            demand[NbTypes-1] = dis_C(gen);
            break;
        }
    }
}

void write(const std::string& folder, const std::string& filename, const std::vector<int>& demand, const std::vector<std::vector<int> >& capacity, const std::vector<int> cost, const int& gamma){
    std::ofstream file;
    file.open(folder+filename, std::ios::out|std::ios::trunc);
    if (file.is_open()){
        file << NbHotels << "," << NbTypes << "," << gamma << std::endl;
        for (int k = 0; k < NbTypes-1; k++){
            file << demand[k] << ",";
        }
        file << demand[NbTypes - 1] << std::endl;
        for (int i = 0; i < NbHotels; i++){
            for (int w = 0; w < NbTypes; w++){
                file << capacity[i][w] << ",";
            }
            file << cost[i] << std::endl;
        }
        file.close();
    } else {
        std::cout << "Failed to open file: " << folder+filename << std::endl;
    }
}