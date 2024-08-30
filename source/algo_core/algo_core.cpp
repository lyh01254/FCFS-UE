#include"algo_core.h"

void Algo::package_solve(const std::string input = ""){
    best_UE();
    worst_UE();
}

void Algo::best_UE(){
    std::vector<double> Q_k(demand);
    std::vector<std::vector<double> > C(capacity);
    std::vector<int> cost_index(NbHotels);
    std::vector<std::vector<std::vector<double> > > assign(NbHotels, std::vector<std::vector<double> > (NbTypes, std::vector<double> (NbTypes, 0)));
    std::iota(cost_index.begin(), cost_index.end(), 0);
    std::sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] < cost[j];});
    double sum_Q = accumulate(Q_k.begin(), Q_k.end(), 0);
    auto it1 = cost_index.begin();
    auto it2 = cost_index.begin();
    while (sum_Q > 0){
        if (it1 != cost_index.end()){
            //* *it1 = i_star
            for (int w = 0; w < NbTypes; w++){
                if (C[*it1][w] > 0){
                    assign[*it1][w][w] = std::min(Q_k[w], C[*it1][w]);
                    C[*it1][w] -= assign[*it1][w][w];
                    Q_k[w] -= assign[*it1][w][w];
                    sum_Q -= assign[*it1][w][w];
                    best_assign += assign[*it1][w][w] * cost[*it1];
                }
            }
            it1++;
        } else {
            //* *it2 = i_star
            for (int w = 0; w < NbTypes; w++){
                if (C[*it2][w] > 0){
                    for (int k = 0; k < NbTypes; k++){
                        if (k != w && Q_k[k] > 0 && C[*it2][w] > 0){
                            assign[*it2][k][w] = std::min(Q_k[k], C[*it2][w]);
                            C[*it2][w] -= assign[*it2][k][w];
                            Q_k[k] -= assign[*it2][k][w];
                            sum_Q -= assign[*it2][k][w];
                            best_assign += assign[*it2][k][w] * cost[*it2];
                            best_misplace += assign[*it2][k][w];
                        }
                    }
                }
            }
            it2++;
        }
    }
    best = best_assign + best_misplace * gamma;    
}

void Algo::worst_UE(){
    best_surplus = 0;
    light_surplus_solve();
    excess_tsp_value = excess_tsp();
    worst = best_surplus + excess_tsp_value * gamma + excess_assignment_cost;   
}

void Algo::set_parameters(const std::string filename = ""){
    if (filename == ""){
        NbHotels = 4;
        NbTypes = 3;
        demand = {10,20,30};
        capacity = {{5,6,6},{7,8,9},{5,6,5},{7,10,3}};
        cost = {2,3,4,5};
        gamma = 10;
    } else {
        try{
            read(NbHotels, filename, 0, 0);
            read(NbTypes, filename, 0, 1);
            read(gamma, filename, 0, 2);
            read(demand, filename, 1, 0, 1, NbTypes-1);
            read(cost, filename, 2, NbTypes, 1+NbHotels, NbTypes);
            read(capacity, filename, 2, 0, 1+NbHotels, NbTypes-1);
        }catch(const char* msg){
            std::cerr << msg << std::endl;
            exit(1);
        }
    }
}

void Algo::pre_process(){
    direct_misplace = 0;
    total_surplus = 0;
    excess_assignment_cost = 0;
    
    for (int k = 0; k < NbTypes; k++){
        Capacity_by_k.push_back(0);
        double temp = 0;
        for (int i = 0; i < NbHotels; i++){
            Capacity_by_k[k] += capacity[i][k];
            temp += capacity[i][k] * cost[i];
        }
        difference.push_back(demand[k] - Capacity_by_k[k]);
        if (difference.back() >= 0){
            is_excess.push_back(true);
            excess_types.push_back(k);
            direct_misplace += difference.back();
            excess_assignment_cost += temp;
        } else {
            is_excess.push_back(false);
            surplus_type.push_back(k);
            total_surplus -= difference.back();
        }
    }    
}

double Algo::excess_tsp(){
    std::vector<double> V_E (1<<excess_types.size(), 0);
    std::vector<double> D (1<<excess_types.size(), 0);
    D[0] = direct_misplace;
    for (int i = 1; i < V_E.size() - 1; i++){
        for (int j = 0; j < excess_types.size(); j++){
            if ((1<<j) & i) { //if the j+1 position of i is 1
                int k = excess_types[j];
                if (D[i] == 0) {
                    D[i] = D[i ^ (1<<j)] - difference[k];
                }
                if (V_E[i] < std::min(D[i], Capacity_by_k[k]) + V_E[i ^ (1<<j)]) {
                    V_E[i] = std::min(D[i], Capacity_by_k[k]) + V_E[i ^ (1<<j)];
                }
            }
        }
    }
    int i = V_E.size()-1;
    for (int j = 0; j < excess_types.size(); j++){
        if (V_E[i] < V_E[i ^ (1<<j)]){
            V_E[i] = V_E[i ^ (1<<j)];
        }
    }
    return V_E.back();
}

double Algo::surplus_tsp(std::vector<double>& worst_assign_by_type){
    std::vector<double> V_S (1<<surplus_type.size(), 0);
    std::vector<double> surplus_S (1<<surplus_type.size(), 0);
    for (int i = 1; i < V_S.size(); i++){
        double D = 0;
        for (int j = 0; j < surplus_type.size(); j++) {
            if ((1<<j) & i) {
                int k = surplus_type[j];
                if (surplus_S[i] == 0) {
                    surplus_S[i] = surplus_S[i ^ (1<<j)] - difference[k];
                    D = std::max(direct_misplace - (total_surplus - surplus_S[i]), 0.0);
                }
                if (V_S[i] < std::min(std::max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)]) {
                    V_S[i] = std::min(std::max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)];
                }
            }
        }
    }
    for (int i = V_S.size()-1; i > 0; ){
        for (int j = 0; j < surplus_type.size(); j++){
            if ((1<<j) & i) {
                int k = surplus_type[j];
                double D = std::max(direct_misplace - (total_surplus - surplus_S[i]), 0.0);
                if (V_S[i] == std::min(std::max(D + difference[k],0.0), demand[k]) + V_S[i ^ (1<<j)]){
                    if (D + difference[k] > 0) {
                        worst_assign_by_type[k] = Capacity_by_k[k];
                    } else {
                        worst_assign_by_type[k] = demand[k] + D;
                    } 
                    i = (i ^ (1<<j));
                }  
            }
        }
    }
    return V_S.back();
}

void Algo::light_solve(const std::vector<int>& sequence, const std::vector<Curve>& v){
    auto it = sequence.end()-1;
    Curve V;
    v2V(V, v[surplus_type[*it]]);
    while (--it >= sequence.begin()){
        int k = surplus_type[*it];
        Curve raised_V;
        raise(raised_V, V, v[k], gamma);
        //cout << "raised" << endl;
        std::vector<Curve> IC_curve(V.get_NbCons());
        inf_convolute(IC_curve, V, v[k]);
        //cout << "IC" << endl;
        Curve new_V;
        join(new_V, raised_V, IC_curve);
        //cout << "joined" << endl;
        V = new_V;
    }
    if (V.value_at(direct_misplace) > best_surplus) {
        best_surplus = V.value_at(direct_misplace);
        best_curve = V;
    }
}

void Algo::light_enumerate(std::vector<int>& sequence, int start, const std::vector<Curve>& v){
    if (start == sequence.size()-1) {
        //display(sequence, "sequence to be checked:");
        light_solve(sequence, v);
    } else {
        int original_start = sequence[start];
        for (int i = start; i < sequence.size(); i++){
            int temp = sequence[i];
            sequence[i] = original_start;
            sequence[start] = temp;
            light_enumerate(sequence, start+1, v);
            sequence[i] = temp;
            sequence[start] = original_start;
        }
    }
}

void Algo::light_surplus_solve(){
    std::vector<Curve> v(NbTypes);
    find_v(v,demand, capacity, cost, gamma);
    std::vector<int> sequence(surplus_type.size());
    iota(sequence.begin(), sequence.end(), 0);
    light_enumerate(sequence, 0, v);
}