#ifndef CURVE_H
#define CURVE_H
#include <vector>
#include "common.h"
#include <algorithm>
#include <numeric>

class Curve{
private:
    int NbSegs;
    int NbEnds;
    int NbConsegs; //number of concave segments
    std::vector<double> x; //end points
    std::vector<double> y;
    std::vector<double> SegLen_x;
    std::vector<double> SegLen_y;
    std::vector<double> slopes;
    std::vector<int> con_index; //starting point index for each concave segment 

public:
    friend void find_v(std::vector<Curve>& v, const std::vector<double>& demand, const std::vector<std::vector<double> >& capacity, const std::vector<double>& cost, const double& gamma);
    friend void raise(Curve& raised_V, const Curve& V, const Curve& v, const double& gamma);
    friend void inf_convolute(std::vector<Curve>& IC_curves, const Curve& V, const Curve& v);
    friend void join(Curve& V, const Curve& raised_V, const std::vector<Curve>& IC_curves);
    friend void v2V(Curve& V, const Curve& v);
    friend void truncate(Curve& truncated_V, const Curve& V, const Curve& v, const double& gamma);
    void display() const;
    void shift(const double& V);
    Curve();
    const int& get_NbEnds();
    const std::vector<double>& get_y(); 
    const int& get_NbCons();
    const double value_at(const double& point) const;
};


#endif