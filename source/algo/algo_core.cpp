#include "algo_core.h"

void Algo::blanket_solve(const std::string input){
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
    //std::cout << "light_surplus_solved!" << std::endl;
    excess_tsp_value = excess_tsp();
    worst = best_surplus + excess_tsp_value * gamma + excess_assignment_cost;   
}

void Algo::set_parameters(const std::string filename){
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
    Capacity_by_k.clear();
    difference.clear();
    is_excess.clear();
    excess_types.clear();
    surplus_type.clear();
    
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

void Algo::worst_assign_UE(){
    //* obtain the worst assignment under UE
    std::vector<double> Q_k(demand);
    std::vector<std::vector<double> > C(capacity);
    std::vector<int> cost_index(NbHotels);
    std::vector<std::vector<std::vector<double> > > assign(NbHotels, std::vector<std::vector<double> > (NbTypes, std::vector<double> (NbTypes, 0)));
    std::iota(cost_index.begin(), cost_index.end(), 0);
    std::sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] > cost[j];});
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
                    worst_assign += assign[*it1][w][w] * cost[*it1];
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
                            worst_assign += assign[*it2][k][w] * cost[*it2];
                        }
                    }
                }
            }
            it2++;
        }
    }
}

void Algo::compute_bounds(){
    //* obtain the worst misplace under UE
    std::vector<int> cost_index(NbHotels);
    std::iota(cost_index.begin(), cost_index.end(), 0);
    std::sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] > cost[j];});
    std::vector<double> worst_assign_by_type(NbTypes, 0);
    for (int k = 0; k < NbTypes; k++){
        if (is_excess[k]){
            worst_assign_by_type[k] = Capacity_by_k[k];
        }
    }
    worst_misplace = excess_tsp() + surplus_tsp(worst_assign_by_type) + direct_misplace;
    worst_ub = worst_assign + worst_misplace * gamma;
    double worst_assign_con = 0;
    for (int k = 0; k < NbTypes; k++){
        if (worst_assign_by_type[k] == Capacity_by_k[k]){
            for (int i = 0; i < NbHotels; i++){
                worst_assign_con += capacity[i][k] * cost[i];
            }
        } else if (worst_assign_by_type[k] < Capacity_by_k[k]){
            auto it3 = cost_index.begin();
            double a_k = worst_assign_by_type[k];
            while (a_k > 0) {
                worst_assign_con += std::min(a_k, capacity[*it3][k]) * cost[*it3];
                a_k -= std::min(a_k, capacity[*it3][k]);
                it3++;
            }
        }
    }
    worst_lb = worst_misplace * gamma + worst_assign_con;
}

void Algo::show_parameters(){
    display(demand, "Demand");
    display(cost, "Cost");
    display(capacity, "Capacity");
    std::cout << "gamma = " << gamma << std::endl;
}

const double Algo::sample(){
    //show_parameters();

    //todo: set up necessary variables 
    int r_NbUsers = std::accumulate(demand.begin(), demand.end(), 0);
    std::vector<int> r_NbResource_by_type(NbTypes);

    for (int k = 0; k < NbTypes; k++){
        r_NbResource_by_type[k] = 0;
        for (int i = 0; i < NbHotels; i++){
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
        for (int i = 0; i < NbHotels; i++){
            std::fill(it, it + capacity[i][k], i);
            it += capacity[i][k];
        }
    }

    std::vector<std::vector<std::vector<double>>> assignment(NbHotels, std::vector<std::vector<double>> (NbTypes, std::vector<double> (NbTypes, 0)));

    // display(r_NbResource_by_type, "r_NbRes_byType");
    // display(r_users, "r_users");
    // display(r_types, "r_types");
    // display(r_resource, "r_resource");

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
            total_cost += 1;
        }
        //* 2.2 sample facility
        std::uniform_int_distribution<> facility_dis(0, r_NbResource_by_type[resource_type]-1);
        facility_index = facility_dis(gen);
        facility = r_resource[resource_type][facility_index];
        //* delete the capacity
        r_resource[resource_type][facility_index] = r_resource[resource_type][r_NbResource_by_type[resource_type]-1];
        r_NbResource_by_type[resource_type] --;
        if (r_NbResource_by_type[resource_type] == 0) {
            //* delete the type
            r_types[type_index] = r_types[r_NbTypes-1];
            index_by_type[r_types[type_index]] = type_index;
            r_NbTypes --;
        }
        assignment[facility][user_type][resource_type]++;
        //total_cost += cost[facility];
    }
    //std::cout << "Cost = " << total_cost << std::endl;
    return total_cost;    
}

void Algo::simulate(const int& NbSamples){
    for (int s = 0; s < NbSamples; s++){
        sampled_cost.push_back(sample());
    }
}

const double Algo::ad_sample(){
    //show_parameters();

    //todo: set up necessary variables 
    int r_NbUsers = std::accumulate(demand.begin(), demand.end(), 0);
    std::vector<int> r_NbResource_by_type(NbTypes);

    for (int k = 0; k < NbTypes; k++){
        r_NbResource_by_type[k] = 0;
        for (int i = 0; i < NbHotels; i++){
            r_NbResource_by_type[k] += capacity[i][k];
        }
    }

    // std::vector<int> r_users(r_NbUsers); //list of all users:  [0,0,0,1,1,2,2,2,2,2]
    // auto it_user = r_users.begin();
    // for (int k = 0; k < NbTypes; k++){
    //     std::fill(it_user, it_user + demand[k], k);
    //     it_user += demand[k];
    // }

    int r_NbTypes = NbTypes;
    std::vector<int> r_types(NbTypes);
    std::iota(r_types.begin(), r_types.end(), 0); //list of types that has remaining capacity
    std::vector<std::vector<int>> r_resource(NbTypes); //list of unit resources by types * location: [[0,0,0,1,1,1],[1,1,2,2,2,2]]

    for (int k = 0; k < NbTypes; k++){
        r_resource[k] = std::vector<int> (r_NbResource_by_type[k]);
        auto it = r_resource[k].begin();
        for (int i = 0; i < NbHotels; i++){
            std::fill(it, it + capacity[i][k], i);
            it += capacity[i][k];
        }
    }

    //*competition ratio
    std::vector<int> counter(NbTypes);
    std::vector<double> ratio(NbTypes);
    for (int k = 0; k < NbTypes; k++){
        if (demand[k] != 0){
            ratio[k] = (r_NbUsers - demand[k])*(r_NbUsers - demand[k]);
            //ratio[k] = (demand[k])*(demand[k]);
        }
    }
    //double sum_ratio = std::accumulate(ratio.begin(), ratio.end(), 0);
    double sum_ratio = 0;
    for (int i = 0; i < NbTypes; i++){
        sum_ratio += ratio[i];
    }
    for (int k = 0; k < NbTypes; k++){
        ratio[k] = ratio[k] / sum_ratio;
    }
    //display(ratio, "ratio");
    std::vector<std::vector<std::vector<double>>> assignment(NbHotels, std::vector<std::vector<double>> (NbTypes, std::vector<double> (NbTypes, 0)));
    // display(r_NbResource_by_type, "r_NbRes_byType");
    // display(r_users, "r_users");
    // display(r_types, "r_types");
    // display(r_resource, "r_resource");

    //todo: sample
    std::random_device rd;
    std::mt19937 gen(rd());

    double total_cost = 0;
    std::vector<int> index_by_type(r_types);

    while (r_NbUsers > 0){
        //todo: sample user
        std::uniform_real_distribution<> user_dis(0, 1);
        double rand = user_dis(gen);
        int user_type;
        double current_ratio = 0;
        for (int i = 0; i < ratio.size(); i++){
            current_ratio += ratio[i];
            if (rand < current_ratio){
                user_type = i;
                counter[user_type]++;
                break;
            }
        }
        r_NbUsers--;
        if (counter[user_type] == demand[user_type]){
            sum_ratio = 1 - ratio[user_type];
            ratio[user_type] = 0;
            if (sum_ratio > 0){
                for (int i = 0; i < NbTypes; i++){
                    ratio[i] /= sum_ratio;
                }
            }
        }

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
            total_cost += 1;
        }
        //* 2.2 sample facility
        std::uniform_int_distribution<> facility_dis(0, r_NbResource_by_type[resource_type]-1);
        facility_index = facility_dis(gen);
        facility = r_resource[resource_type][facility_index];
        //* delete the capacity
        r_resource[resource_type][facility_index] = r_resource[resource_type][r_NbResource_by_type[resource_type]-1];
        r_NbResource_by_type[resource_type] --;
        if (r_NbResource_by_type[resource_type] == 0) {
            //* delete the type
            r_types[type_index] = r_types[r_NbTypes-1];
            index_by_type[r_types[type_index]] = type_index;
            r_NbTypes --;
        }
        assignment[facility][user_type][resource_type]++;
        //total_cost += cost[facility];
    }
    //std::cout << "Cost = " << total_cost << std::endl;
    return total_cost;
}

const double Algo::customed_sample(const std::vector<int>& order){
    //show_parameters();

    //todo: set up necessary variables 
    std::vector<int> r_NbResource_by_type(NbTypes);

    for (int k = 0; k < NbTypes; k++){
        r_NbResource_by_type[k] = 0;
        for (int i = 0; i < NbHotels; i++){
            r_NbResource_by_type[k] += capacity[i][k];
        }
    }

    // std::vector<int> r_users(r_NbUsers); //list of all users:  [0,0,0,1,1,2,2,2,2,2]
    // auto it_user = r_users.begin();
    // for (int k = 0; k < NbTypes; k++){
    //     std::fill(it_user, it_user + demand[k], k);
    //     it_user += demand[k];
    // }

    int r_NbTypes = NbTypes;
    std::vector<int> r_types(NbTypes);
    std::iota(r_types.begin(), r_types.end(), 0); //list of types that has remaining capacity
    std::vector<std::vector<int>> r_resource(NbTypes); //list of unit resources by types * location: [[0,0,0,1,1,1],[1,1,2,2,2,2]]

    for (int k = 0; k < NbTypes; k++){
        r_resource[k] = std::vector<int> (r_NbResource_by_type[k]);
        auto it = r_resource[k].begin();
        for (int i = 0; i < NbHotels; i++){
            std::fill(it, it + capacity[i][k], i);
            it += capacity[i][k];
        }
    }

    //display(ratio, "ratio");
    std::vector<std::vector<std::vector<double>>> assignment(NbHotels, std::vector<std::vector<double>> (NbTypes, std::vector<double> (NbTypes, 0)));
    // display(r_NbResource_by_type, "r_NbRes_byType");
    // display(r_users, "r_users");
    // display(r_types, "r_types");
    // display(r_resource, "r_resource");

    //todo: sample
    std::random_device rd;
    std::mt19937 gen(rd());

    double total_cost = 0;
    std::vector<int> index_by_type(r_types);
    std::vector<int> count(NbTypes);
    std::vector<int> count_C(NbTypes);
    int current = 0;
 
    while (current < NbTypes){
        //todo: sample user
        int user_type = order[current];
        if (count[user_type] < demand[user_type]){
            count[user_type]++;
        } else {
            current++;
            continue;
        }
        //std::cout << "current = " << current << std::endl;

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
            total_cost += 1;
        }
        //* 2.2 sample facility
        std::uniform_int_distribution<> facility_dis(0, r_NbResource_by_type[resource_type]-1);
        facility_index = facility_dis(gen);
        facility = r_resource[resource_type][facility_index];
        //* delete the capacity
        r_resource[resource_type][facility_index] = r_resource[resource_type][r_NbResource_by_type[resource_type]-1];
        r_NbResource_by_type[resource_type] --;
        if (r_NbResource_by_type[resource_type] == 0) {
            //* delete the type
            r_types[type_index] = r_types[r_NbTypes-1];
            index_by_type[r_types[type_index]] = type_index;
            r_NbTypes --;
        }
        assignment[facility][user_type][resource_type]++;
        //total_cost += cost[facility];
    }
    //std::cout << "Cost = " << total_cost << std::endl;
    return total_cost;
}

double Algo::cplex_solve(){
    IloEnv env;
    IloModel ue_model(env);
    int nbHotels = 1;
    std::vector<std::vector<IloNumVarArray> > alpha(nbHotels, std::vector<IloNumVarArray>(NbTypes));
    //*variables
    for (int i = 0; i < nbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            alpha[i][k] = IloNumVarArray(env, NbTypes, 0, 1);
            for (int w = 0; w < NbTypes; w++){
                alpha[i][k][w].setName(("alpha(" + std::to_string(i) + "," + std::to_string(k) + "," + std::to_string(w) + ")").c_str());
            }
        }
    }
    IloIntVarArray r(env, NbTypes, 0, 1);
    IloNumVarArray u(env, NbTypes, 0, NbTypes);
    std::vector<IloIntVarArray> v(NbTypes);
    for (int k = 0; k < NbTypes; k++){
        r[k].setName(("r("+std::to_string(k)+")").c_str());
        u[k].setName(("u("+std::to_string(k)+")").c_str());
        v[k] = IloIntVarArray(env, NbTypes, 0, 1);
        for (int w = 0; w < NbTypes; w++){
            v[k][w].setName(("v("+std::to_string(k)+","+std::to_string(w)+")").c_str());
        }
    }

    //*Objective
    IloNumExpr expr_misplacement(env);
    for (int i = 0; i < nbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            for (int w = 0; w < NbTypes; w++){
                if (w != k){
                    expr_misplacement += demand[k] * alpha[i][k][w];
                }
            }
        }
    }
    IloObjective obj_misplacement(env, expr_misplacement);

    //*constraints
    IloRangeArray conservation(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        for (int i = 0; i < nbHotels; i++){
            for (int w = 0; w < NbTypes; w++){               
                lhs += alpha[i][k][w];
            }
        }
        conservation.add(lhs == 1);
        conservation[k].setName(("con(" + std::to_string(k) + ")").c_str());
    }
    ue_model.add(conservation);

    std::vector<IloRangeArray> cap;
    for (int i = 0; i < nbHotels; i++){
        IloRangeArray cap_i(env);
        for (int w = 0; w < NbTypes; w++){
            IloNumExpr lhs(env);
            for (int k = 0; k < NbTypes;k++){
                lhs += demand[k] * alpha[i][k][w];
            }
            cap_i.add(lhs <= Capacity_by_k[w]);
            cap_i[w].setName(("cap("+std::to_string(i)+","+std::to_string(w)+")").c_str());
        }
        cap.push_back(cap_i);
        ue_model.add(cap_i);
    }

    IloRangeArray alpha_r(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        for (int i = 0; i < nbHotels; i++){
            for (int w = 0; w < NbTypes; w++){
                if (w != k){
                    lhs += alpha[i][k][w];
                }
            }
        }
        lhs -= r[k];
        alpha_r.add(lhs <= 0);
        alpha_r[k].setName(("alpha_r("+std::to_string(k)+")").c_str());
    }
    ue_model.add(alpha_r);

    IloRangeArray r_alpha(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        lhs += Capacity_by_k[k] * r[k];
        for (int i = 0; i < nbHotels; i++){
            for (int w = 0; w < NbTypes; w++){
                lhs -= demand[w] * alpha[i][w][k];
            }
        }
        r_alpha.add(lhs <= 0);
        r_alpha[k].setName(("r_alpha("+std::to_string(k)+")").c_str());
    }
    ue_model.add(r_alpha);

    std::vector<IloRangeArray> u_v;
    std::vector<IloRangeArray> alpha_v;
    for (int k = 0; k < NbTypes; k++){
        for (int w = 0; w < NbTypes; w++){
            IloRangeArray uv_kw(env);
            IloRangeArray alphav_kw(env);
            if (k != w){
                uv_kw.add(u[k] - u[w] + NbTypes * v[k][w] <= NbTypes - 1);
                uv_kw[uv_kw.getSize()-1].setName(("u_v("+std::to_string(k)+","+std::to_string(w)+")").c_str());
                IloNumExpr lhs(env);
                for (int i = 0; i < nbHotels; i++){
                    lhs += alpha[i][k][w];
                }
                alphav_kw.add(lhs - v[k][w] <= 0);
                alphav_kw[alphav_kw.getSize()-1].setName(("alpha_v("+std::to_string(k)+","+std::to_string(w)+")").c_str());
            }
            u_v.push_back(uv_kw);
            alpha_v.push_back(alphav_kw);
            ue_model.add(uv_kw);
            ue_model.add(alphav_kw);
        }
    }

    //*cplex
    int time_limit = 3600;
    IloCplex ue_cplex(ue_model);

    //ue_cplex.setOut(env.getNullStream());
    ue_cplex.setParam(IloCplex::Param::Threads, 1);

    ue_cplex.setParam(IloCplex::Param::TimeLimit, time_limit);
    ue_model.add(obj_misplacement);
    obj_misplacement.setSense(IloObjective::Maximize);
    double gap = -1;
    if (ue_cplex.solve()) {
        gap = ue_cplex.getMIPRelativeGap();
    } 
    env.end();  

    return gap;                  
}