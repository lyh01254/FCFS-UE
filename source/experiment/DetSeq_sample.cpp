#include "algo_core.h"

std::string result_folder = "results/sample/";
std::string data_folder = "data/sample/";
int sample_size = 10000;
void compute(const std::string& file_name, 
             const std::vector<int>& order, 
             const std::vector<std::vector<double>> scheme, 
             std::vector<double>& cost);

int main(){
    //todo: prepare instance file list
    std::vector<std::string> file_list = {"demo.csv"};
    std::vector<std::vector<std::vector<double>>> schemes = {{{10, 60, 110, 300}},
                                                            {{20, 60, 100, 300}},
                                                            {{30, 60, 90, 300}},
                                                            {{40, 60, 80, 300}},
                                                            {{50, 60, 70, 300}},
                                                            {{60, 60, 60, 300}}};
    std::vector<std::vector<int>> orders = {{0,1,2,3},
                                            {0,2,1,3},
                                            {1,0,2,3},
                                            {1,2,0,3},
                                            {2,0,1,3},
                                            {2,1,0,3}};
    //*loop over instances
    for (int i = 0; i < file_list.size(); i++){
        for (int o = 0; o < orders.size(); o++){
            std::vector<std::vector<double>> costs(schemes.size(), std::vector<double>(sample_size));
            for (int s = 0; s < schemes.size(); s++){
                //* for each instance, compute(instane_file, sample_size);
                compute(file_list[i], orders[o], schemes[s], costs[s]);
            }
            display(costs, "Cost");
            
            //* write result to file 
            std::ofstream file;
            std::string file_name;
            for (int j = 0; j < orders[o].size(); j++){
                file_name += std::to_string(orders[o][j]);
            }
            file_name += ".csv";
            file.open(result_folder+file_name, std::ios::out|std::ios::trunc);
            if (file.is_open()){
                for (int s = 0; s < schemes.size(); s++){
                    std::string header;
                    for (int j = 0; j < schemes[s][0].size()-1; j++){
                        header += std::to_string((int) schemes[s][0][j]);
                        header += "-";
                    }
                    header += std::to_string((int) schemes[s][0][schemes[s][0].size()-1]);
                    //std::cout << "header: " << header << std::endl;
                    file << header << ",";
                }
                file << std::endl;
                for (int i = 0; i < sample_size; i++){
                    for (int s = 0; s < schemes.size(); s++){
                        file << std::to_string(costs[s][i]) << ","; 
                    }
                    file << std::endl;
                }
                file.close();
            } else {
                std::cout << "Failed to open file " << result_folder+file_name << std::endl;
            }
        }
    }
    return 0;
}

void compute(const std::string& file_name, const std::vector<int>& order, const std::vector<std::vector<double>> scheme, std::vector<double>& cost){
    //* initiate an algo object
    Algo algo;
    std::ifstream test_file;
    test_file.open(data_folder+file_name, std::ios::in);
    if (!test_file.is_open()){
        std::cout << "Failed to open file " << file_name << std::endl;
    }
    test_file.close();
    algo.set_parameters(data_folder+file_name);
    algo.setCapacity(scheme);
    algo.show_parameters();
    algo.pre_process();

    // //* calculate the best-case UE performance
    // algo.best_UE();
    // std::cout << "Best UE solved." << std::endl;

    // //* calculate the worst-case UE performance
    // algo.worst_UE();
    // std::cout << "Worst UE solved." << std::endl;

    //* sample and compute cost
    for (int i = 0; i < sample_size; i++){
        cost[i] = algo.customed_sample(order);
    } 
}