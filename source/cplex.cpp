#include "myIOS.h"
#include "common.h"
#include "ilcplex/ilocplex.h"
#include<chrono>
#include<fstream>

ILOSTLBEGIN

int NbHotels, NbTypes;

string root = "data/totally_random/";
string output = "results/cplex_detail.csv";

int main(int argc, char* argv[]){
    ofstream file;
    file.open(output, ios::out|ios::trunc);
    file << "I,K,instance, so_assignment, so_misplace, so_obj, best_assign, worst_assign, best_misplace, worst_misplace, best, worst, time_best, time_worst" << endl;
    file.close();
    for (NbHotels = 20; NbHotels <= 40; NbHotels += 20){
        for (NbTypes = 4; NbTypes <= 5; NbTypes++){
            for (int instance = 1; instance <= 5; instance++){
                string file_name = root+to_string(NbHotels) + "_" + to_string(NbTypes) + "_" + to_string(instance) + ".csv"; 
                double gamma;
                vector<double> demand;
                vector<vector<double> > capacity;
                vector<double> cost;
                vector<double> Capacity_by_k;
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
                double so_assignment = 0;
                double so_misplace = 0;
                double so_obj = 0;
                double best_assign = 0;
                double worst_assign = 0;
                double best_misplace = 0;
                double worst_misplace = 0;
                double best = 0;
                double worst = 0;
                double time_best = 0;
                double time_worst = 0;

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

                //*Objective
                IloNumExpr expr_obj(env);
                IloNumExpr expr_misplacement(env);
                IloNumExpr expr_assignment(env);
                for (int i = 0; i < NbHotels; i++){
                    for (int k = 0; k < NbTypes; k++){
                        for (int w = 0; w < NbTypes; w++){
                            expr_obj += cost[i] * demand[k] * alpha[i][k][w];
                            expr_assignment += cost[i] * demand[k] * alpha[i][k][w];
                            if (w != k){
                                expr_obj += gamma * demand[k] * alpha[i][k][w];
                                expr_misplacement += demand[k] * alpha[i][k][w];
                            }
                        }
                    }
                }
                IloObjective obj(env, expr_obj);
                IloObjective obj_misplacement(env, expr_misplacement);
                IloObjective obj_assignment(env, expr_assignment);

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

                //*cplex
                IloCplex ue_cplex(ue_model);
                IloCplex so_cplex(so_model);
                ue_cplex.setOut(env.getNullStream());
                so_cplex.setOut(env.getNullStream());

                //* obtain the SO result
                obj.setSense(IloObjective::Minimize);
                so_model.add(obj);
                if (so_cplex.solve()){
                    so_assignment = so_cplex.getValue(expr_assignment);
                    so_misplace = so_cplex.getValue(expr_misplacement);
                    so_obj = so_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " SO infeasible." << endl;
                }
                so_model.remove(obj);

                //* obtain the best/worst assignment under UE
                ue_model.add(obj_assignment);
                obj_assignment.setSense(IloObjective::Minimize);
                if (ue_cplex.solve()){
                    best_assign = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }
                obj_assignment.setSense(IloObjective::Maximize);
                if (ue_cplex.solve()){
                    worst_assign = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }
                ue_model.remove(obj_assignment);

                //* obtain the best/worst misplace under UE
                ue_model.add(obj_misplacement);
                obj_misplacement.setSense(IloObjective::Minimize);
                if (ue_cplex.solve()){
                    best_misplace = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }
                obj_misplacement.setSense(IloObjective::Maximize);
                if (ue_cplex.solve()){
                    worst_misplace = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }
                ue_model.remove(obj_misplacement);

                // * obtain the best obj under UE, timing
                ue_model.add(obj);
                obj.setSense(IloObjective::Minimize);
                auto start = chrono::high_resolution_clock::now();
                if (ue_cplex.solve()){
                    auto end = chrono::high_resolution_clock::now();
                    chrono::duration<double, std::nano> elapsed = end - start;
                    time_best = elapsed.count();
                    best = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }

                //* obtain the worst obj under UE, timing
                obj.setSense(IloObjective::Maximize);
                start = chrono::high_resolution_clock::now();
                if (ue_cplex.solve()){
                    auto end = chrono::high_resolution_clock::now();
                    chrono::duration<double, std::nano> elapsed = end - start;
                    time_worst = elapsed.count();
                    worst = ue_cplex.getObjValue();
                } else {
                    cout << "I = " << NbHotels << " K = " << NbTypes << " instance = " << instance << " UE infeasible." << endl;
                }
                file.open(output, ios::out|ios::app);
                file << NbHotels << "," << NbTypes << "," << instance << "," << so_assignment << "," << so_misplace << "," << so_obj << "," << best_assign << "," << worst_assign << "," << best_misplace << "," << worst_misplace << "," << best << "," << worst << "," << time_best << "," << time_worst << endl;
                file.close();
                env.end();                
            }
        }
    }




    return 0;
}
