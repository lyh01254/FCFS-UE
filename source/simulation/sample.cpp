#include"common.h"
#include<numeric>
#include<algorithm>
#include<random>

//* Read-only Inputs: NbTypes, NbNodes, Demand, Capacity, cost parameters
int NbTypes, NbNodes;
std::vector<int> demand;
std::vector<std::vector<int>> capacity;
std::vector<double> cost;
double gamma;


void demo_setting();
void show_parameters();

int main(){
    //todo: read the read-only parameter from file
    demo_setting();
    show_parameters();

    //todo: set up necessary variables 
    int r_NbUsers = std::accumulate(demand.begin(), demand.end(), 0);
    std::vector<int> r_NbResource_by_type(NbTypes);

    for (int k = 0; k < NbTypes; k++){
        r_NbResource_by_type[k] = 0;
        for (int i = 0; i < NbNodes; i++){
            r_NbResource_by_type[k] += capacity[i][k];
        }
    }

    std::vector<int> r_users(r_NbUsers); //list of all users:  [0,0,0,1,1,2,2,2,2,2]
    auto it_user = r_users.begin();
    for (int k = 0; k < NbTypes; k++){
        std::fill(it_user, it_user + demand[k], k);
        it_user += demand[k];
    }

    int r_NbTypes = NbTypes;
    std::vector<int> r_types(NbTypes);
    std::iota(r_types.begin(), r_types.end(), 0); //list of types that has remaining capacity
    std::vector<std::vector<int>> r_resource(NbTypes); //list of unit resources by types * location: [[0,0,0,1,1,1],[1,1,2,2,2,2]]

    for (int k = 0; k < NbTypes; k++){
        r_resource[k] = std::vector<int> (r_NbResource_by_type[k]);
        auto it = r_resource[k].begin();
        for (int i = 0; i < NbNodes; i++){
            std::fill(it, it + capacity[i][k], i);
            it += capacity[i][k];
        }
    }

    std::vector<std::vector<std::vector<double>>> assignment(NbNodes, std::vector<std::vector<double>> (NbTypes, std::vector<double> (NbTypes, 0)));

    display(r_NbResource_by_type, "r_NbRes_byType");
    display(r_users, "r_users");
    display(r_types, "r_types");
    display(r_resource, "r_resource");

    //todo: sample
    std::random_device rd;
    std::mt19937 gen(rd());

    double total_cost = 0;
    std::vector<int> index_by_type(r_types);

    while (r_NbUsers > 0){
        //* 1. sample user
        std::uniform_int_distribution<> user_dis(0, r_NbUsers-1);
        int user_index = user_dis(gen);
        int user_type = r_users[user_index];
        r_users[user_index] = r_users[r_NbUsers-1];
        r_NbUsers --;

        //* 2. sample resource
        int type_index, resource_type, facility_index, facility;
        //* 2.1 sample resource type
        if (r_NbResource_by_type[user_type] > 0){
            resource_type = user_type;
            type_index = index_by_type[resource_type];
        } else {
            std::uniform_int_distribution<> type_dis(0, r_NbTypes-1);
            type_index = type_dis(gen);
            resource_type = r_types[type_index];
            total_cost += gamma;
        }
        //* 2.2 sample facility
        std::uniform_int_distribution<> facility_dis(0, r_NbResource_by_type[resource_type]-1);
        facility_index = facility_dis(gen);
        facility = r_resource[resource_type][facility_index];
        //* delete the capacity
        r_resource[resource_type][facility_index] = r_resource[resource_type][r_NbResource_by_type[resource_type]-1];
        r_NbResource_by_type[resource_type] --;
        if (r_NbResource_by_type[resource_type] == 0) {
            r_types[type_index] = r_types[r_NbTypes-1];
            index_by_type[r_types[type_index]] = type_index;
            r_NbTypes --;
        }
        assignment[facility][user_type][resource_type]++;
        total_cost += cost[facility];
    }
    std::cout << "Cost = " << total_cost << std::endl;
    return 0;
}

void demo_setting(){
    NbNodes = 4;
    NbTypes = 3;
    demand = {10,20,30};
    capacity = {{5,6,6},{7,8,9},{5,6,5},{7,10,3}};
    cost = {2,3,4,5};
    gamma = 10;
}

void show_parameters(){
    display(demand, "Demand");
    display(cost, "Cost");
    display(capacity, "Capacity");
    std::cout << "gamma = " << gamma << std::endl;
}




