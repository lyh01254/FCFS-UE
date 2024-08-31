#ifndef ALGO_CORE_H
#define ALGO_CORE_H
#include"myIOS.h"
#include"curve.h"
#include<fstream>
#include<random>

class Algo{
private:
    //*primary parameters
    int NbHotels, NbTypes;
    double gamma;
    std::vector<double> demand;
    std::vector<std::vector<double> > capacity;
    std::vector<double> cost;

    //*intermediate parameters
    std::vector<double> Capacity_by_k;
    std::vector<bool> is_excess;
    std::vector<int> excess_types;
    std::vector<int> surplus_type;
    double direct_misplace;
    double total_surplus;
    std::vector<double> difference;
    double excess_assignment_cost;

    //*Calculation results
    Curve best_curve;
    double best_surplus;
    double best_assign = 0;
    double worst_assign = 0;
    double best_misplace = 0;
    double worst_misplace = 0;
    double best = 0;
    double worst = 0;
    double excess_tsp_value = 0;
    double worst_ub = 0;
    double worst_lb = 0;
    std::vector<double> sampled_cost;

public:
    //default constructor: since all members have their default constructor, Algo need not have one.
    void set_parameters(const std::string filename = "");
    void pre_process();
    void light_solve(const std::vector<int>& sequence, const std::vector<Curve>& v);
    void light_enumerate(std::vector<int>& sequence, int start, const std::vector<Curve>& v);
    void light_surplus_solve();
    double excess_tsp();
    double surplus_tsp(std::vector<double>& worst_assign_by_type);
    void blanket_solve(const std::string input = "");
    void best_UE();
    void worst_UE();
    void simulate(const int& NbSamples);
    const double sample();
    void show_parameters();
    void compute_bounds();
    void worst_assign_UE();

    //*primary parameters: getters and setters
    int getNbHotels() const { return NbHotels; }
    void setNbHotels(int value) { NbHotels = value; }

    int getNbTypes() const { return NbTypes; }
    void setNbTypes(int value) { NbTypes = value; }

    double getGamma() const { return gamma; }
    void setGamma(double value) { gamma = value; }

    const std::vector<double>& getDemand() const { return demand; }
    void setDemand(const std::vector<double>& value) { demand = value; }

    const std::vector<std::vector<double>>& getCapacity() const { return capacity; }
    void setCapacity(const std::vector<std::vector<double>>& value) { capacity = value; }

    const std::vector<double>& getCost() const { return cost; }
    void setCost(const std::vector<double>& value) { cost = value; }

    //*intermediate parameters or results: only getters
    const std::vector<double>& getCapacityByK() const { return Capacity_by_k; }
    const std::vector<bool>& getIsExcess() const { return is_excess; }
    const std::vector<int>& getExcessTypes() const { return excess_types; }
    const std::vector<int>& getSurplusType() const { return surplus_type; }
    double getDirectMisplace() const { return direct_misplace; }
    double getTotalSurplus() const { return total_surplus; }
    const std::vector<double>& getDifference() const { return difference; }
    const Curve& getBestCurve() const { return best_curve; }
    double getBestSurplus() const { return best_surplus; }
    double getBestAssign() const { return best_assign; }
    double getWorstAssign() const { return worst_assign; }
    double getBestMisplace() const { return best_misplace; }
    double getWorstMisplace() const { return worst_misplace; }
    double getBest() const { return best; }
    double getWorst() const { return worst; }
    double getExcessTspValue() const { return excess_tsp_value; }
    double getWorstUb() const { return worst_ub; }
    double getWorstLb() const { return worst_lb; }
};

#endif