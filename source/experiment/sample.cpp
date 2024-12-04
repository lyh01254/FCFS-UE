#include "algo_core.h"

/********************************************
input file name: I{I}_K{K}_{id}.csv
output file name: I{I}_K{K}_{id}.csv
********************************************/
std::string result_folder = "results/sample/";
std::string data_folder = "data/sample/";
int sample_size = 10000;
void compute(std::string file_name);

int main(){
    //todo: prepare instance file list
    std::vector<std::string> file_list = {"demo.csv"};

    //*loop over instances
    for (int i = 0; i < file_list.size(); i++){
        //* for each instance, compute(instane_file, sample_size);
        compute(file_list[i]);
    }
    return 0;
}

void compute(std::string file_name){
    //* initiate an algo object
    Algo algo;
    std::ifstream test_file;
    test_file.open(data_folder+file_name, std::ios::in);
    if (!test_file.is_open()){
        std::cout << "Failed to open file " << file_name << std::endl;
    }
    test_file.close();
    algo.set_parameters(data_folder+file_name);
    algo.show_parameters();
    algo.pre_process();

    //* sample result
    std::vector<double> sampled_cost(sample_size);

    //* calculate the best-case UE performance
    algo.best_UE();
    std::cout << "Best UE solved." << std::endl;

    //* calculate the worst-case UE performance
    algo.worst_UE();
    std::cout << "Worst UE solved." << std::endl;

    //* sample and compute cost
    for (int i = 0; i < sample_size; i++){
        sampled_cost[i] = algo.customed_sample({0,1,2,3});
    } 

    //* write result to file 
    std::ofstream file;
    file.open(result_folder+file_name, std::ios::out|std::ios::trunc);
    if (file.is_open()){
        file << "Cost," << std::to_string(algo.getBest()) << "," << std::to_string(algo.getWorst()) << std::endl;
        for (int i = 0; i < sample_size; i++){
            file << std::to_string(sampled_cost[i]) << std::endl;
        }
        file.close();
    } else {
        std::cout << "Failed to open file " << result_folder+file_name << std::endl;
    }
}