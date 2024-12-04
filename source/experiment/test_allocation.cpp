#include "algo_core.h"

/* test the worst-case performance of different allocation policy
 consider the heuristic rule, the proportional rule and random
*/

std::string data_folder = "data/totally_random/";
std::string result_folder = "results/";
std::vector<int> I_set = {20, 40, 60, 80, 100};
std::vector<int> K_set = {4, 6, 8, 10, 12, 14, 16};
std::vector<int> instance_set = {1, 2, 3, 4, 5};
// std::vector<int> I_set = {20};
// std::vector<int> K_set = {4};
// std::vector<int> instance_set = {1, 2};
void compute(const std::string& file_name, std::vector<double>& result);
int main(){
    std::vector<std::vector<double>> result(I_set.size()*K_set.size()*instance_set.size());
    int count = 0;
    for (const int& I : I_set){
        std::string file_name;
        for (const int& K : K_set){
            for (const int& instance : instance_set){
                file_name = std::to_string(I) + "_" + std::to_string(K) + "_" + std::to_string(instance) + ".csv";
                //todo: compute the result for random, heuristic and proportional rules.
                std::cout << file_name << std::endl;
                compute(file_name, result[count]);
                count++;
            }
        }
    }
    std::ofstream file;
    file.open(result_folder+"allocation.csv", std::ios::out|std::ios::trunc);
    if (!file.is_open()){
        std::cout << "Failed to open file." << std::endl;
    } else {
        file << "I, K, instance, best, random, proportional, heuristic" << std::endl;
        count = 0;
        for (const int& I : I_set){
            for (const int& K : K_set){
                for (const int& instance : instance_set){
                    file << I << "," << K << "," << instance << "," << result[count][0] << "," << result[count][1] << "," << result[count][3] << "," << result[count][2] << std::endl;
                    count++;
                }
            }
        }
    }
    file.close();
    return 0;
}

void compute(const std::string& file_name, std::vector<double>& result){
    Algo algo;
    std::ifstream test_file;
    test_file.open(data_folder+file_name, std::ios::in);
    if (!test_file.is_open()){
        std::cout << "Failed to open file " << file_name << std::endl;
    }
    test_file.close();
    algo.set_parameters(data_folder+file_name);
    algo.pre_process();
    double best = algo.getDirectMisplace();
    result.push_back(best);
    //todo: random
    algo.compute_bounds();
    double worst_random = algo.getWorstMisplace();
    result.push_back(worst_random);

    //todo: compute total capacity for E and S, respectively
    double E_capacity = 0, S_capacity = 0;
    for (int k = 0; k < algo.getNbTypes(); k++){
        if (algo.getIsExcess()[k]){
            E_capacity += algo.getCapacityByK()[k];
        } else {
            S_capacity += algo.getCapacityByK()[k];
        }
    }

    std::vector<int> excess_types = algo.getExcessTypes();
    //todo: for E, even excess demand
    double remain_excess = algo.getDirectMisplace();
    int remain_Etypes = excess_types.size();
    std::vector<double> E_demand_list;
    for (const int& k : excess_types){
        E_demand_list.push_back(algo.getDemand()[k]);
    }

    std::sort(E_demand_list.begin(), E_demand_list.end(), [](double i, double j){return i < j;});

    std::vector<double> ideal_capacity;
    double ideal_excess = remain_excess / remain_Etypes;

    double weight = 0;
    double bound1 = 0;
    for (const double& Q : E_demand_list){
        remain_Etypes --;
        if (Q < ideal_excess){
            ideal_capacity.push_back(0);
            remain_excess -= Q;
            ideal_excess += remain_excess / remain_Etypes;
            bound1 += Q * weight;
        } else {
            ideal_capacity.push_back(Q - ideal_excess);
            remain_excess -= ideal_excess;
            bound1 += ideal_excess * weight;
        }
        weight++;
    }

    //todo: for E, even capacity
    std::vector<double> even_capacity;
    double bound2 = 0;
    double remain_capacity = E_capacity;
    remain_Etypes = excess_types.size();
    double ideal = remain_capacity / remain_Etypes;
    for (const double& Q : E_demand_list){
        remain_Etypes--;
        if (Q < ideal) {
            even_capacity.push_back(Q);
            remain_capacity -= Q;
            ideal = remain_capacity / remain_Etypes;
        } else {
            even_capacity.push_back(ideal);
            remain_capacity -= ideal;
        }
    }
    bound2 = E_capacity - even_capacity[0];

    
    //todo: for S, even surplus type
    double ideal_surplus = algo.getTotalSurplus() / algo.getSurplusType().size();

    std::vector<double> surplus_capacity;
    std::vector<double> S_demand_list;
    for (const int& k : algo.getSurplusType()){
        S_demand_list.push_back(algo.getDemand()[k]);
        surplus_capacity.push_back(algo.getDemand()[k]+ideal_surplus);
    }
    std::vector<double> altered_demand = E_demand_list;
    altered_demand.insert(altered_demand.end(), S_demand_list.begin(), S_demand_list.end());
    std::vector<std::vector<double>> heuristic_capacity(1);
    if (bound1 <= bound2) {
        heuristic_capacity[0] = ideal_capacity;
    } else {
        heuristic_capacity[0] = even_capacity;
    }
    heuristic_capacity[0].insert(heuristic_capacity[0].end(), surplus_capacity.begin(), surplus_capacity.end());

    Algo altered_algo;
    altered_algo.setNbHotels(1);
    altered_algo.setNbTypes(algo.getNbTypes());
    altered_algo.setDemand(altered_demand);
    altered_algo.setCapacity(heuristic_capacity);
    altered_algo.setCost({0});
    altered_algo.setGamma(1);
    altered_algo.pre_process();
    altered_algo.compute_bounds();
    double worst_heuristic = altered_algo.getWorstMisplace();
    result.push_back(worst_heuristic);

    //todo: proportional 
    std::vector<double> E_capacity_prop;
    std::vector<double> S_capacity_prop;
    double E_total = std::accumulate(E_demand_list.begin(), E_demand_list.end(), 0);
    double S_total = std::accumulate(S_demand_list.begin(), S_demand_list.end(), 0);
    for (const double& Q : E_demand_list){
        E_capacity_prop.push_back(E_capacity * (Q/E_total));
    }
    for (const double& Q : S_demand_list){
        S_capacity_prop.push_back(S_capacity * (Q/S_total));
    }
    std::vector<std::vector<double>> capacity_prop(1);
    capacity_prop[0] = E_capacity_prop;
    capacity_prop[0].insert(capacity_prop[0].end(), S_capacity_prop.begin(), S_capacity_prop.end());
    display(capacity_prop, "capacity_prop");
    altered_algo.setCapacity(capacity_prop);
    altered_algo.show_parameters();
    altered_algo.pre_process();
    std::cout << "best = " << altered_algo.getDirectMisplace() << std::endl;
    altered_algo.compute_bounds();
    double worst_prop = altered_algo.getWorstMisplace();
    result.push_back(worst_prop);
}

