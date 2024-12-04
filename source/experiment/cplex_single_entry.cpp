#include "algo_core.h"
#include <chrono>

int main(){
    std::vector<int> K_set = {14,16,18,20};
    std::vector<int> instance_set = {1,2,3,4,5,6,7,8,9,10};
    std::ofstream ave_file;
    std::string result_file = "results/single6.csv";
    ave_file.open(result_file, std::ios::out|std::ios::trunc);
    ave_file << "K,instance,time,gap" << std::endl;
    ave_file.close();
    for (auto k = K_set.begin(); k < K_set.end(); k++){
        double best_time = __DBL_MAX__;
        double worst_time = 0;
        double mean_time = 0;
        double best_gap = 9999;
        double worst_gap = 0;
        double mean_gap = 0;
        for (auto instance = instance_set.begin(); instance < instance_set.end(); instance++){
            Algo algo;
            algo.set_parameters("data/single1/" + std::to_string(*k) + "_" + std::to_string(*instance) + ".csv");
            algo.pre_process();
            auto start = std::chrono::high_resolution_clock::now();
            double gap = 0;
            try{
                gap = algo.cplex_solve();
            }
            catch(IloException& oops){
                std::cerr << oops.getMessage() << std::endl;
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            double time = elapsed.count();
            // if (gap < best_gap) {
            //     best_gap = gap;
            // }
            // if (gap > worst_gap) {
            //     worst_gap = gap;
            // }
            // if (time < best_time) {
            //     best_time = time;
            // }
            // if (time > worst_time) {
            //     worst_time = time;
            // }
            // mean_gap = (mean_gap * (*instance - 1) + gap) / *instance;
            // mean_time = (mean_time * (*instance - 1) + time) / *instance;
            ave_file.open(result_file, std::ios::out|std::ios::app);
            ave_file << *k << "," << *instance << "," << time << "," << gap << std::endl;
            ave_file.close();
        }
    }
    return 0;
}

