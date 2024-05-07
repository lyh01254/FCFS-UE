#include "curve.h"

int Curve::get_NbSegs() const{
    return NbSegs;
}

int Curve::get_NbEnds() const{
    return NbEnds;
}

int Curve::get_NbConsegs() const{
    return NbConsegs;
}

const std::vector<double>& Curve::get_x() const{
    return x;
}

const std::vector<double>& Curve::get_y() const{
    return y;
}

const std::vector<double>& Curve::get_slopes() const{
    return slopes;
}

const std::vector<int>& Curve::get_con_index() const{
    return con_index;
}

void find_v(std::vector<Curve>& v, const std::vector<double>& demand, const std::vector<std::vector<double> >& capacity, const std::vector<double>& cost, const double& gamma){
    int NbHotels = capacity.size();
    int NbTypes = demand.size();
    std::vector<int> sorted_index(NbHotels, 0);
    std::iota(sorted_index.begin(), sorted_index.end(), 0);
    std::sort(sorted_index.begin(), sorted_index.end(), [&](int i, int j){return cost[i] > cost[j];});
    std::vector<std::vector<double>> C(capacity);
    for (int k = 0; k < NbTypes; k++){
        double sum_C = 0;
        for (int i = 0; i < NbHotels; i++){
            sum_C += C[i][k];
        }
        double Q = demand[k];
        auto it = sorted_index.begin();
        double x = Q;
        double y = 0;
        while (Q > 0){
            if (Q >= C[*it][k]){
                y += cost[*it] * C[*it][k];
                Q -= C[*it][k];
                it++;
            } else {
                y += cost[*it] * Q;
                C[*it][k] -= Q;
                Q = 0;
            }
        }
        v[k].x.push_back(x);
        v[k].y.push_back(y);
        double previous_slope = -1;
        while (true){
            double slope_to_add = (cost[*it] + gamma);
            x += C[*it][k];
            y += slope_to_add * C[*it][k];
            if (slope_to_add != previous_slope){
                v[k].SegLen_x.push_back(C[*it][k]);
                v[k].SegLen_y.push_back(slope_to_add * C[*it][k]);
                v[k].x.push_back(x);
                v[k].y.push_back(y);
                v[k].slopes.push_back(slope_to_add);
                if (x < sum_C){
                    ++it;
                } else {
                    break;
                }
            } else {
                v[k].x.back() = x;
                v[k].y.back() = y;
                v[k].SegLen_x.back() += C[*it][k];
                v[k].SegLen_y.back() += slope_to_add * C[*it][k];
            }
        }
        v[k].NbEnds = v[k].x.size();
        v[k].NbSegs = v[k].slopes.size();
        v[k].NbConsegs = 1;
        v[k].con_index.push_back(0);
        v[k].con_index.push_back(v[k].NbSegs);
    }
}

void raise(Curve& raised_V, const Curve& V, const Curve& v, const double& gamma){
    double raised_x = v.x[v.NbSegs] - v.x[0];
    double raised_y = v.y[v.NbSegs] + V.y[0];
    double raised_slope = 0;
    raised_V.x.push_back(raised_x);
    raised_V.y.push_back(raised_y);
    double Dt = v.x[0];
    int idx = 0;
    raised_V.con_index.push_back(idx);
    while (idx < V.NbEnds){
         if (V.x[idx] < Dt && V.x[idx+1] > Dt){
            //todo: seglen_x and seglen_y
            //*First handle the raised segment
            double new_slope = V.slopes[idx] + gamma;
            double new_len = Dt - V.x[idx];
            raised_x += new_len; //the next x
            raised_y += new_len * new_slope; //the next y
            if (new_slope > raised_slope && idx > 0) { //if convexity changes
                raised_V.con_index.push_back(idx); //current end is a split
            }
            raised_V.x.push_back(raised_x);
            raised_V.y.push_back(raised_y);
            raised_V.slopes.push_back(new_slope);
            raised_V.SegLen_x.push_back(new_len);

            //*Then handle the remaining segment
            new_slope = V.slopes[idx];
            new_len = V.x[idx+1] - Dt;
            raised_x += new_len; //the next x
            raised_y += new_len * new_slope; //the next y
            //*Note: convexity must maintain
            raised_slope = new_slope; //the next slope //*Note: new_slope must != old_slope
            raised_V.x.push_back(raised_x);
            raised_V.y.push_back(raised_y);
            raised_V.slopes.push_back(new_slope);
            raised_V.SegLen_x.push_back(new_len);
            ++idx;
        } else { 
            //todo: a special case where old_slope == new slope
            double new_slope = V.slopes[idx];
            if (V.x[idx+1] <= Dt){
                new_slope += gamma;
            } 
            raised_x += V.SegLen_x[idx]; //the next x
            raised_y += V.SegLen_x[idx] * new_slope; //the next y
            if (new_slope > raised_slope && idx > 0){ //if convexity changes
                raised_V.con_index.push_back(idx); //current end is a split
            } 
            raised_slope = new_slope; //the next slope //*Note: new_slope must != old_slope
            raised_V.x.push_back(raised_x);
            raised_V.y.push_back(raised_y);
            raised_V.slopes.push_back(raised_slope);
            raised_V.SegLen_x.push_back(V.SegLen_x[idx]);
            ++idx;         
        }
    }
};

void inf_convolute(std::vector<Curve>& IC_curves, const Curve& V, const Curve& v){
    int End_v = v.x.back(); //last end index of v
    for (int i = 0; i < V.NbConsegs; i++){ //for each concave segment
        int idx_v = 0; //index of ends for v
        int idx_V = V.con_index[i]; //index of ends for V's concave segment i
        int End_V = V.con_index[i+1];
        double x_V = V.x[idx_V]; //starting x of concave segment 
        double x_v = v.x[0]; //starting x of v; i.e., Dt
        double x = x_V; //starting x of IC_curve
        double y = v.y[0]+V.y[i]; //starting y of IC_curve
        IC_curves[i].x.push_back(x); //push back the starting x
        IC_curves[i].y.push_back(y); //push back the starting y
        //*slope re-arrange
        while (idx_v < End_v || idx_V < End_V){ //there's still slope to insert
            if (idx_v == End_v || V.slopes[idx_V] >= v.slopes[idx_v]){ //condition for inserting the next slope in V
                //todo: a new segment
                double slope_to_add = v.slopes[idx_v];
                x += v.SegLen[idx_v];
                y += 
            } else { //otherwise, insert the next slope in v
                if (IC_curves[i].slopes.back() != v.slopes[idx_v]){
                    //todo: extension of the previous
                } else {
                    //todo: a new segment
                }
            }
        }
    }
    int v_idx = 0;
    int V_idx = 0;
    while (V_idx < V.NbEnds || v_idx < v.NbEnds){
        if (V_idx >= V.NbEnds - 1 || v.slopes[v_idx] >= V.slopes[V_idx]){
            
        }
    }
};

void join(Curve& V, const Curve& raised_V, const std::vector<Curve>& IC_curves){

};

void v2V(Curve& V, const Curve& v){
    V.NbEnds = v.NbEnds;
    V.NbSegs = v.NbSegs;
    V.NbConsegs = v.NbConsegs;
    V.con_index = v.con_index;
    V.x = v.x;
    V.y = v.y;
    V.slopes = v.slopes;
    for (int i = 0; i < V.NbEnds; i++){
        V.x[i] -= v.x[0];
    }
}
