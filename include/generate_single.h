#ifndef GEN_SINGLE
#define GEN_SINGLE

#include<iostream>
#include<vector>
#include<random>
#include<fstream>
#include"common.h"

class Instance_generator{
private:
    int NbHotels, NbTypes, gamma;
    int capacity_lb, capacity_ub;
    int demand_lb, demand_ub;
    std::vector<int> cost;
    std::vector<std::vector<int>> capacity;
    std::vector<int> demand;

public:
    Instance_generator(const int& I, const int& K);
    void set_capacity_lb(const int& lb);
    void set_capacity_ub(const int& ub);
    void set_demand_lb(const int& lb);
    void set_demand_ub(const int& ub);
    bool validate_demand();
    void refresh_demand();
    void refresh_capacity();
    void refresh_everything();
    void write_to(std::string file_name) const;
    void refresh_demand_at(const int& k, const int& lb, const int& ub);
};

#endif