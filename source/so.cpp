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

string file_name = "data/instance/100_15_5.csv";

int main(){
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
    display(demand, "Demand");
    display(cost, "Cost");
    display(capacity, "Capacity");
    cout << "Data input passes." << endl;

    IloEnv env;
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
    cout << "Variables done!" << endl;
    //*Objective
    IloNumExpr expr_obj(env);
    for (int i = 0; i < NbHotels; i++){
        for (int k = 0; k < NbTypes; k++){
            for (int w = 0; w < NbTypes; w++){
                expr_obj += cost[i] * demand[k] * alpha[i][k][w];
                if (w != k){
                    expr_obj += gamma * demand[k] * alpha[i][k][w];
                }
            }
        }
    }
    IloObjective obj(env, expr_obj);
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
        so_model.add(cap_i);
    }
    cout << "Capacity done!" << endl;
    //*cplex
    IloCplex so_cplex(so_model);
    so_cplex.exportModel("so_model.lp");
    if (so_cplex.solve()){
        cout << "SO value = " << so_cplex.getObjValue() << endl;
    } else{
        cout << "SO is infeasible." << endl;
    }
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
    display(cost_index, "cost_index");
    double sum_Q = accumulate(Q_k.begin(), Q_k.end(), 0);
    auto it1 = cost_index.begin();
    auto it2 = cost_index.begin();
    double so_cost = 0;
    while (sum_Q > 0){
        if (it1 != cost_index.end() && cost[*it1] < cost[*it2]+gamma){
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
    cout << "so_cost = " << so_cost << endl;
    // for (int i = 0; i < NbHotels; i++){
    //     display(assign[i], "assignment i = " + to_string(i));
    // }

    return 0;
}

