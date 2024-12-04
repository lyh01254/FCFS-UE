#include "algo_core.h"
#include <chrono>

int main(){
    std::vector<int> K_set = {4,6,8,10,12,14,16,18,20};
    std::vector<int> instance_set = {1,2,3,4,5,6,7,8,9,10};
    std::ofstream ave_file;
    ave_file.open("results/single_algo.csv", std::ios::out|std::ios::trunc);
    ave_file << "K,best_time,worst_time,mean_time,best_gap,worst_gap,mean_gap" << std::endl;
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
            algo.compute_bounds();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            double time = elapsed.count();
            if (gap < best_gap) {
                best_gap = gap;
            }
            if (gap > worst_gap) {
                worst_gap = gap;
            }
            if (time < best_time) {
                best_time = time;
            }
            if (time > worst_time) {
                worst_time = time;
            }
            mean_gap = (mean_gap * (*instance - 1) + gap) / *instance;
            mean_time = (mean_time * (*instance - 1) + time) / *instance;
        }
        ave_file.open("results/single_algo.csv", std::ios::out|std::ios::app);
        ave_file << *k << "," << best_time << "," << worst_time << "," << mean_time << "," << best_gap << "," << worst_gap << "," << mean_gap << std::endl;
        ave_file.close();
    }
    return 0;
}

