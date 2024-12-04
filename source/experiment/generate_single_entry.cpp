#include "generate_single.h"

int main(){
    std::vector<int> K_set = {4,6,8,10,12,14,16,18,20};
    int NbInstances = 10;
    for (auto k = K_set.begin(); k < K_set.end(); k++){
        Instance_generator instance(1, *k);
        instance.set_capacity_lb(1);
        instance.set_capacity_ub(100);
        for (int i = 0; i < NbInstances; i++){
            instance.refresh_everything();
            instance.write_to("data/single1/"+std::to_string(*k)+"_"+std::to_string(i+1)+".csv");
        }
    }
    return 0;
}