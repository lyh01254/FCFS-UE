#include "generate_single.h"

Instance_generator::Instance_generator(const int& I, const int& K){
    NbHotels = I;
    NbTypes = K;
    gamma = 1;
    cost = std::vector<int> (NbHotels, 0);
    capacity = std::vector<std::vector<int>> (NbHotels, std::vector<int> (NbTypes, 0));
    demand = std::vector<int> (NbTypes, 0);
    capacity_lb = 0;
    capacity_ub = 0;
    demand_lb = 0;
    demand_ub = 0;
}

void Instance_generator::set_capacity_lb(const int& lb) {
    capacity_lb = lb;
}

void Instance_generator::set_capacity_ub(const int& ub) {
    capacity_ub = ub;
}

void Instance_generator::set_demand_lb(const int& lb) {
    demand_lb = lb;
}

void Instance_generator::set_demand_ub(const int& ub) {
    demand_ub = ub;
}

void Instance_generator::refresh_capacity(){
    if (capacity_lb >= 0 && capacity_ub > 0){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_C(capacity_lb, capacity_ub);
        for (int k = 0; k < NbTypes; k++){
            for (int i = 0; i < NbHotels; i++){
                capacity[i][k] = dis_C(gen);
            }
        }
    } else {
        std::string msg = "No legal bound set for capacity. lb = " + std::to_string(capacity_lb) + ", ub = " + std::to_string(capacity_ub);
        throw msg;
    }
}

void Instance_generator::refresh_demand_at(const int& k, const int& lb, const int& ub){
    if (k >= 0 && k < NbTypes){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_Q(lb, ub);
        demand[k] = dis_Q(gen);
    } else {
        throw "k out of range";
    }
}

void Instance_generator::refresh_demand(){
    int sum_C = std::accumulate(capacity[0].begin(), capacity[0].end(), 0);
    if (sum_C > 0) {
        do{
            for (int k = 0; k < NbTypes/2; k++){
                refresh_demand_at(k, capacity[0][k], 1.5*capacity[0][k]);
            }
            for (int k = NbTypes/2; k < NbTypes; k++){
                refresh_demand_at(k, 0.5*capacity[0][k], capacity[0][k]);
            }
        }while (!validate_demand());
    } else {
        throw "No legal capacity set.";
    }
}

bool Instance_generator::validate_demand(){
    int sum_C = std::accumulate(capacity[0].begin(), capacity[0].end(), 0);
    int sum_Q = std::accumulate(demand.begin(), demand.end(), 0);
    if (sum_Q > 0) {
        if (sum_C > sum_Q){
            return true;
        } else {
            int sum_Q1 = std::accumulate(demand.begin(), demand.end()-1, 0);
            if (sum_C > sum_Q1){
                refresh_demand_at(NbTypes-1, 0, sum_C - sum_Q1);
                return true;
            } else {
                return false;
            }
        }
    }else{
        return false;
    }
}

void Instance_generator::refresh_everything(){
    refresh_capacity();
    refresh_demand();
}

void Instance_generator::write_to(std::string file_name) const{
    std::ofstream file;
    file.open(file_name, std::ios::out|std::ios::trunc);
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
        std::cout << "Failed to open file: " << file_name << std::endl;
    } 
}