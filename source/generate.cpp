#include<iostream>
#include<vector>
#include<random>
#include<fstream>
#include"common.h"

int NbHotels, NbTypes, gamma;

void generate_cost(std::vector<int>& cost, const double& lb, const double& ub);
int generate_gamma(const std::vector<int>& cost);
void generate_capacity(std::vector<std::vector<int>>& capacity, const int& min_K, const double& lb, const double& ub);
void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int>>& capacity, const double& excess_prob, const double& excess_rate, const double& surplus_rate);
void write(const std::string& folder, const std::string& filename, const std::vector<int>& demand, const std::vector<std::vector<int>>& capacity, const std::vector<int> cost, const int& gamma);

int main(){
    double MIN_COST = 1;
    double MAX_COST = 50;
    double MIN_CAPACITY = 10;
    double MAX_CAPACITY = 100;
    double EXCESS_RATE = 0.5;
    double SURPLUS_RATE = 0.5;
    double EXCESS_PROB = 0.4;
    for (NbHotels = 20; NbHotels <= 100; NbHotels+=20){
        for (NbTypes = 4; NbTypes <= 15; NbTypes++){
            for (int instance = 1; instance <= 5; instance++){
                int MIN_K = std::max(NbTypes/2, 4);
                std::vector<int> cost(NbHotels, 0);
                std::vector<int> demand(NbTypes, 0);
                std::vector<std::vector<int>> capacity(NbHotels, std::vector<int>(NbTypes, 0));
                generate_cost(cost, MIN_COST, MAX_COST);
                int gamma = generate_gamma(cost);
                generate_capacity(capacity, MIN_K, MIN_CAPACITY, MAX_CAPACITY);
                generate_demand(demand, capacity, EXCESS_PROB, EXCESS_RATE, SURPLUS_RATE);
                std::string filename = std::to_string(NbHotels) + "_" + std::to_string(NbTypes) + "_" + std::to_string(instance) + ".csv";
                std::string folder = "data/totally_random/";
                write(folder, filename, demand, capacity, cost, gamma);
            }
        }
    }
    return 0;
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

void generate_capacity(std::vector<std::vector<int>>& capacity, const int& min_K, const double& lb, const double& ub){
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

void generate_demand(std::vector<int>& demand, const std::vector<std::vector<int>>& capacity, const double& excess_prob, const double& excess_rate, const double& surplus_rate) {
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
    while(true){
        int sum_Q = 0;
        //* todo: first type is excess
        demand[0] = dis_0(gen);
        sum_Q += demand[0];
        //*todo: random types
        for (int k = 1; k < NbTypes - 1; k++){
            if (dis(gen) <= 0.4) { // excess
                std::uniform_int_distribution<> dis_C(C_k[k], (1+excess_rate)*C_k[k]);
                demand[k] = dis_C(gen);
            } else {
                std::uniform_int_distribution<> dis_C((1-surplus_rate)*C_k[0], C_k[0]);
                demand[k] = dis_C(gen);
            }
            sum_Q += demand[k];
        }
        // std::cout << "sum_Q = " << sum_Q << std::endl;
        // std::cout << "sum_C = " << sum_C << std::endl;
        // std::cin.get();
        if (sum_Q < sum_C) {
            std::uniform_int_distribution<> dis_C(0, sum_C-sum_Q);
            demand[NbTypes-1] = dis_C(gen);
            break;
        }
    }
}

void write(const std::string& folder, const std::string& filename, const std::vector<int>& demand, const std::vector<std::vector<int>>& capacity, const std::vector<int> cost, const int& gamma){
    std::ofstream file;
    file.open(folder+filename, std::ios::out|std::ios::trunc);
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
}