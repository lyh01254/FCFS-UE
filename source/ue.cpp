#include "myIOS.h"
#include "common.h"
#include "ilcplex/ilocplex.h"
#include <algorithm>
#include <numeric>

ILOSTLBEGIN

int NbHotels, NbTypes;
double gamma;
vector<double> demand;
vector<vector<double> > capacity;
vector<double> cost;
vector<double> Capacity_by_k;


string root = "data/instance/";

int main(int argc, char* argv[]){
    string file_name;
    if (argc > 1){
        file_name = root + argv[1];
    }else{
        file_name = "data/demo.csv";
    }
    try{
        read(NbHotels, file_name, 0, 0);
        read(NbTypes, file_name, 0, 1);
        read(gamma, file_name, 0, 2);
        read(demand, file_name, 1, 0, 1, NbTypes-1);
        read(cost, file_name, 2, NbTypes, 1+NbHotels, NbTypes);
        read(capacity, file_name, 2, 0, 1+NbHotels, NbTypes-1);
    }catch(const char* msg){
        cerr << msg << endl;
        exit(1);
    }
    for (int k = 0; k < NbTypes; k++){
        Capacity_by_k.push_back(0);
        for (int i = 0; i < NbHotels; i++){
            Capacity_by_k[k] += capacity[i][k];
        }
    }
    gamma = 1;
    display(demand, "Demand");
    display(cost, "Cost");
    cout << "gamma = " << gamma << endl;
    display(capacity, "Capacity");
    display(Capacity_by_k, "Capacity_by_k");
    cout << "Data input passes." << endl;

    //gamma = 1;


    IloEnv env;
    IloModel ue_model(env);
    IloModel so_model(env);
    vector<vector<IloNumVarArray> > alpha(NbHotels, vector<IloNumVarArray>(NbTypes));
    //*variables
    for (int i = 0; i < NbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            alpha[i][k] = IloNumVarArray(env, NbTypes, 0, 1);
            for (int w = 0; w < NbTypes; w++){
                alpha[i][k][w].setName(("alpha(" + to_string(i) + "," + to_string(k) + "," + to_string(w) + ")").c_str());
            }
        }
    }
    IloIntVarArray r(env, NbTypes, 0, 1);
    IloNumVarArray u(env, NbTypes, 0, NbTypes);
    vector<IloIntVarArray> v(NbTypes);
    for (int k = 0; k < NbTypes; k++){
        r[k].setName(("r("+to_string(k)+")").c_str());
        u[k].setName(("u("+to_string(k)+")").c_str());
        v[k] = IloIntVarArray(env, NbTypes, 0, 1);
        for (int w = 0; w < NbTypes; w++){
            v[k][w].setName(("v("+to_string(k)+","+to_string(w)+")").c_str());
        }
    }
    cout << "Variables done!" << endl;
    //*Objective
    IloNumExpr expr_obj(env);
    IloNumExpr expr_misplacement(env);
    for (int i = 0; i < NbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            for (int w = 0; w < NbTypes; w++){
                expr_obj += cost[i] * demand[k] * alpha[i][k][w];
                if (w != k){
                    expr_obj += gamma * demand[k] * alpha[i][k][w];
                    expr_misplacement += demand[k] * alpha[i][k][w];
                }
            }
        }
    }
    IloObjective obj(env, expr_obj);
    IloObjective obj_misplacement(env, expr_misplacement);
    ue_model.add(obj);
    so_model.add(obj);
    cout << "Objective done!" << endl;
    //*constraints
    IloRangeArray conservation(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        for (int i = 0; i < NbHotels; i++){
            for (int w = 0; w < NbTypes; w++){               
                lhs += alpha[i][k][w];
            }
        }
        conservation.add(lhs == 1);
        conservation[k].setName(("con(" + to_string(k) + ")").c_str());
    }
    ue_model.add(conservation);
    so_model.add(conservation);
    cout << "Conservation done!" << endl;

    vector<IloRangeArray> cap;
    for (int i = 0; i < NbHotels; i++){
        IloRangeArray cap_i(env);
        for (int w = 0; w < NbTypes; w++){
            IloNumExpr lhs(env);
            for (int k = 0; k < NbTypes;k++){
                lhs += demand[k] * alpha[i][k][w];
            }
            cap_i.add(lhs <= capacity[i][w]);
            cap_i[w].setName(("cap("+to_string(i)+","+to_string(w)+")").c_str());
        }
        cap.push_back(cap_i);
        ue_model.add(cap_i);
        so_model.add(cap_i);
    }
    cout << "Capacity done!" << endl;

    IloRangeArray alpha_r(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        for (int i = 0; i < NbHotels; i++){
            for (int w = 0; w < NbTypes; w++){
                if (w != k){
                    lhs += alpha[i][k][w];
                }
            }
        }
        lhs -= r[k];
        alpha_r.add(lhs <= 0);
        alpha_r[k].setName(("alpha_r("+to_string(k)+")").c_str());
    }
    ue_model.add(alpha_r);
    cout << "alpha_r done!" << endl;

    IloRangeArray r_alpha(env);
    for (int k = 0; k < NbTypes; k++){
        IloNumExpr lhs(env);
        lhs += Capacity_by_k[k] * r[k];
        for (int i = 0; i < NbHotels; i++){
            for (int w = 0; w < NbTypes; w++){
                lhs -= demand[w] * alpha[i][w][k];
            }
        }
        r_alpha.add(lhs <= 0);
        r_alpha[k].setName(("r_alpha("+to_string(k)+")").c_str());
    }
    ue_model.add(r_alpha);
    cout << "r_alpha done!" << endl;

    vector<IloRangeArray> u_v;
    vector<IloRangeArray> alpha_v;
    for (int k = 0; k < NbTypes; k++){
        for (int w = 0; w < NbTypes; w++){
            IloRangeArray uv_kw(env);
            IloRangeArray alphav_kw(env);
            if (k != w){
                uv_kw.add(u[k] - u[w] + NbTypes * v[k][w] <= NbTypes - 1);
                uv_kw[uv_kw.getSize()-1].setName(("u_v("+to_string(k)+","+to_string(w)+")").c_str());
                IloNumExpr lhs(env);
                for (int i = 0; i < NbHotels; i++){
                    lhs += alpha[i][k][w];
                }
                alphav_kw.add(lhs - v[k][w] <= 0);
                alphav_kw[alphav_kw.getSize()-1].setName(("alpha_v("+to_string(k)+","+to_string(w)+")").c_str());
            }
            u_v.push_back(uv_kw);
            alpha_v.push_back(alphav_kw);
            ue_model.add(uv_kw);
            ue_model.add(alphav_kw);
        }
    }
    cout << "u_v done!" << endl;
    obj.setSense(IloObjective::Maximize);

    //*cplex
    IloCplex ue_cplex(ue_model);
    IloCplex so_cplex(so_model);
    obj.setSense(IloObjective::Maximize);
    ue_cplex.setOut(env.getNullStream());
    //ue_cplex.exportModel("ue_model.lp");
    //obj.setSense(IloObjective::Minimize);
    //so_cplex.exportModel("so_model.lp");
    if (ue_cplex.solve()){
        if (obj.getSense() == 1){
            cout << "Best UE = " << ue_cplex.getObjValue() << endl;
        } else {
            cout << "Worst UE = " << ue_cplex.getObjValue() << endl;
        }
    } else{
        cout << "UE is infeasible." << endl;
    }
    double misplaced_demand = 0;
    for (int i = 0; i < NbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            for (int w = 0; w < NbTypes; w++){
                if (k != w){
                    misplaced_demand += ue_cplex.getValue(alpha[i][k][w])*demand[k];
                }
            }
        }
    }
    cout << "Misplaced demand at worst UE = " << misplaced_demand << endl;

    ue_model.remove(obj);
    ue_model.add(obj_misplacement);
    obj_misplacement.setSense(IloObjective::Maximize);
    if (ue_cplex.solve()){
    if (obj.getSense() == 1){
        cout << "Best misplacement = " << ue_cplex.getObjValue() << endl;
    } else {
        cout << "Worst misplacement = " << ue_cplex.getObjValue() << endl;
    }
    } else{
        cout << "UE is infeasible." << endl;
    }

    ue_model.add(expr_misplacement >= ue_cplex.getObjValue());
    ue_model.remove(obj_misplacement);
    ue_model.add(obj);
    if (ue_cplex.solve()){
        if (obj.getSense() == 1){
            cout << "Best UE = " << ue_cplex.getObjValue() << endl;
        } else {
            cout << "Worst UE = " << ue_cplex.getObjValue() << endl;
        }
    } else{
        cout << "UE is infeasible." << endl;
    }

    misplaced_demand = 0;
    for (int i = 0; i < NbHotels; i++){
    for (int k = 0; k < NbTypes; k++){
        for (int w = 0; w < NbTypes; w++){
            if (k != w){
                misplaced_demand += ue_cplex.getValue(alpha[i][k][w])*demand[k];
            }
        }
    }
    }
    cout << "Misplaced demand at worst UE = " << misplaced_demand << endl;

    // if (so_cplex.solve()){
    //     cout << "SO value = " << so_cplex.getObjValue() << endl;
    // } else{
    //     cout << "SO is infeasible." << endl;
    // }
    // vector<vector<vector<double> > > cplex_assign(NbHotels, vector<vector<double> > (NbTypes, vector<double> (NbTypes, 0)));
    // for (int i = 0; i < NbHotels; i++){
    //     for (int k = 0; k < NbTypes; k++){
    //         for (int w = 0; w < NbTypes; w++){
    //             cplex_assign[i][k][w] = so_cplex.getValue(alpha[i][k][w]) * demand[k];
    //         }
    //     }
    //     display(cplex_assign[i], "cplex_assign i = " + to_string(i));
    // }


    env.end();

    vector<double> Q_k(demand);
    vector<vector<double> > C(capacity);
    vector<int> cost_index(NbHotels);
    vector<vector<vector<double> > > assign(NbHotels, vector<vector<double> > (NbTypes, vector<double> (NbTypes, 0)));
    iota(cost_index.begin(), cost_index.end(), 0);
    sort(cost_index.begin(), cost_index.end(), [&](int i, int j){return cost[i] < cost[j];});
    //display(cost_index, "cost_index");
    double sum_Q = accumulate(Q_k.begin(), Q_k.end(), 0);
    auto it1 = cost_index.begin();
    auto it2 = cost_index.begin();
    double so_cost = 0;
    while (sum_Q > 0){
        if (it1 != cost_index.end()){
            //* *it1 = i_star
            for (int w = 0; w < NbTypes; w++){
                if (C[*it1][w] > 0){
                    assign[*it1][w][w] = min(Q_k[w], C[*it1][w]);
                    C[*it1][w] -= assign[*it1][w][w];
                    Q_k[w] -= assign[*it1][w][w];
                    sum_Q -= assign[*it1][w][w];
                    so_cost += assign[*it1][w][w] * cost[*it1];
                }
            }
            it1++;
        } else {
            //* *it2 = i_star
            for (int w = 0; w < NbTypes; w++){
                if (C[*it2][w] > 0){
                    for (int k = 0; k < NbTypes; k++){
                        if (k != w && Q_k[k] > 0 && C[*it2][w] > 0){
                            assign[*it2][k][w] = min(Q_k[k], C[*it2][w]);
                            C[*it2][w] -= assign[*it2][k][w];
                            Q_k[k] -= assign[*it2][k][w];
                            sum_Q -= assign[*it2][k][w];
                            so_cost += assign[*it2][k][w] * (cost[*it2] + gamma);
                        }
                    }
                }
            }
            it2++;
        }
    }
    //cout << "greedy_cost = " << so_cost << endl;
    // for (int i = 0; i < NbHotels; i++){
    //     display(assign[i], "assignment i = " + to_string(i));
    // }

    return 0;
}

