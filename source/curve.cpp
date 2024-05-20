#include "curve.h"
using namespace std;

const int& Curve::get_NbEnds(){
    return NbEnds;
}

const std::vector<double>& Curve::get_y(){
    return y;
} 

const int& Curve::get_NbCons(){
    return NbConsegs;
}

Curve::Curve(){
    NbEnds = 0;
    NbSegs = 0;
    NbConsegs = 0;
}

void Curve::display() const{
    cout << "NbEnds = " << NbEnds << endl;
    cout << "NbSegs = " << NbSegs << endl;
    cout << "NbCons = " << NbConsegs << endl;
    ::display(x, "x");
    ::display(y, "y");
    ::display(slopes, "k");
    ::display(SegLen_x, "len_x");
    ::display(SegLen_y, "len_y");
    ::display(con_index, "con_idx");
    cout << endl;
}

void find_v(std::vector<Curve>& v, const std::vector<double>& demand, const std::vector<std::vector<double> >& capacity, const std::vector<double>& cost, const double& gamma){
    int NbHotels = capacity.size();
    int NbTypes = demand.size();
    std::vector<int> sorted_index(NbHotels, 0);
    std::iota(sorted_index.begin(), sorted_index.end(), 0);
    std::sort(sorted_index.begin(), sorted_index.end(), [&](int i, int j){return cost[i] > cost[j];});
    std::vector<std::vector<double>> C(capacity);
    display(sorted_index);
    cout << "ready to find v." << endl;
    for (int k = 0; k < NbTypes; k++){
        cout << "type " << k << " begin." << endl;
        double sum_C = 0;
        for (int i = 0; i < NbHotels; i++){
            sum_C += C[i][k];
        }
        double Q = demand[k];
        if (sum_C <= Q){
            cout << "This is an excess type" << endl;
            v[k].x.push_back(Q-sum_C); //x[0] = truncate point
            v[k].x.push_back(sum_C); //x[1] = sum_C
            double initial_y = 0;
            for (int i = 0; i < NbHotels; i++){
                initial_y += cost[i] * capacity[i][k];
            }
            v[k].y.push_back(initial_y); //y[0] = initial y
            continue;
        } 
        auto it = sorted_index.begin();
        double x = Q;
        double y = 0;
        cout << "start calculating initial point" << endl;
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
        cout << "initial point found." << endl;
        v[k].x.push_back(x);
        v[k].y.push_back(y);
        double previous_slope = -1;
        cout << "initial point set." << endl;
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
    //the first end of the raised curve
    double raised_x = v.x[v.NbSegs] - v.x[0];
    double raised_y = v.y[v.NbSegs] + V.y[0];
    double raised_slope = 0;
    raised_V.x.push_back(raised_x);
    raised_V.y.push_back(raised_y);
    raised_V.con_index.push_back(0);

    double Dt = v.x[0]; //the threshold Dt
    int idx = 0;
   //cout << "start to raise" << endl;
    //V.display();
    while (idx < V.NbEnds - 1){ //
    //cout << "idx = " << idx << endl;
         if (V.x[idx] < Dt && V.x[idx+1] > Dt){
            //*First handle the raised segment
            double new_slope = V.slopes[idx] + gamma;
            double len_x = Dt - V.x[idx];
            double len_y = len_x * new_slope;
            raised_x += len_x; //the next x
            raised_y += len_y; //the next y
            if (new_slope > raised_slope && idx > 0) { //if convexity changes
                raised_V.con_index.push_back(idx); //current end is a split
            }
            raised_V.x.push_back(raised_x);
            raised_V.y.push_back(raised_y);
            raised_V.slopes.push_back(new_slope);
            raised_V.SegLen_x.push_back(len_x);
            raised_V.SegLen_y.push_back(len_y);

            //*Then handle the remaining segment
            new_slope = V.slopes[idx];
            len_x = V.x[idx+1] - Dt;
            len_y = len_x * new_slope;
            raised_x += len_x; //the next x
            raised_y += len_y; //the next y
            //*Note: convexity must maintain
            raised_slope = new_slope; //the next slope //*Note: new_slope must != old_slope
            raised_V.x.push_back(raised_x);
            raised_V.y.push_back(raised_y);
            raised_V.slopes.push_back(new_slope);
            raised_V.SegLen_x.push_back(len_x);
            raised_V.SegLen_y.push_back(len_y);
            ++idx;
        } else { 
            double new_slope = V.slopes[idx];
            if (V.x[idx+1] <= Dt){
                new_slope += gamma;
            } 
            double len_x = V.SegLen_x[idx];
            double len_y = len_x * new_slope;
            raised_x += len_x; //the next x
            raised_y += len_y; //the next y
            if (new_slope > raised_slope && idx > 0){ //if convexity changes
                raised_V.con_index.push_back(idx); //current end is a split
            } 
            if (new_slope != raised_slope){ //there is a special case where new_slope == old_slope
                raised_slope = new_slope; //the next slope 
                raised_V.x.push_back(raised_x);
                raised_V.y.push_back(raised_y);
                raised_V.slopes.push_back(raised_slope);
                raised_V.SegLen_x.push_back(len_x);
                raised_V.SegLen_y.push_back(len_y);
            } else {
                raised_V.x.back() = raised_x;
                raised_V.y.back() = raised_y;
                raised_V.SegLen_x.back() += len_x;
                raised_V.SegLen_y.back() += len_y;
            }
            ++idx;         
        }
    }
    raised_V.NbEnds = raised_V.x.size();
    raised_V.NbSegs = raised_V.NbEnds - 1;
    raised_V.con_index.push_back(idx);
    raised_V.NbConsegs = raised_V.con_index.size() - 1;
};

void inf_convolute(std::vector<Curve>& IC_curves, const Curve& V, const Curve& v){
    int End_v = v.NbSegs; //last end index of v
    for (int i = 0; i < V.NbConsegs; i++){ //for each concave segment
        //cout << "inf convolution i = " << i << endl;
        int idx_v = 0; //iterative index of ends for v
        int idx_V = V.con_index[i]; //iterative index of ends for V's concave segment i
        int End_V = V.con_index[i+1]; //final end index of V's concave segment i
        double x = V.x[idx_V]; //starting x of IC_curve
        double y = v.y[0]+V.y[idx_V]; //starting y of IC_curve
        IC_curves[i].x.push_back(x); //push back the starting x
        IC_curves[i].y.push_back(y); //push back the starting y
        //*slope re-arrange
        double len_x, len_y, slope_to_add;
        while (true){ //there's still slope to insert
            if (idx_v == End_v) { 
                if (idx_V == End_V) {
                    break;
                } else { //insert V
                    len_x = V.SegLen_x[idx_V];
                    len_y = V.SegLen_y[idx_V];
                    slope_to_add = V.slopes[idx_V];
                    ++idx_V;
                    //cout << "insert V; idx_V upadted to " << idx_V << endl;
                }
            } else if (idx_V == End_V) { // insert v
                len_x = v.SegLen_x[idx_v];
                len_y = v.SegLen_y[idx_v];
                slope_to_add = v.slopes[idx_v];
                ++idx_v;
                //cout << "insert v; idx_v upadted to " << idx_v << endl;
            } else if (V.slopes[idx_V] >= v.slopes[idx_v]){ //insert V
                len_x = V.SegLen_x[idx_V];
                len_y = V.SegLen_y[idx_V];
                slope_to_add = V.slopes[idx_V];
                ++idx_V;
                //cout << "insert V; idx_V upadted to " << idx_V << endl;
            } else { //insert v
                len_x = v.SegLen_x[idx_v];
                len_y = v.SegLen_y[idx_v];
                slope_to_add = v.slopes[idx_v];
                ++idx_v;
                //cout << "insert v; idx_v upadted to " << idx_v << endl;
            }
            x += len_x; //next x value
            y += len_y; //next y value
            if (IC_curves[i].slopes.empty() || IC_curves[i].slopes.back() != slope_to_add) { //add a new segment
                IC_curves[i].x.push_back(x);
                IC_curves[i].y.push_back(y);
                IC_curves[i].SegLen_x.push_back(len_x);
                IC_curves[i].SegLen_y.push_back(len_y);
                IC_curves[i].slopes.push_back(slope_to_add);
            } else { //extend the previous segment
                IC_curves[i].x.back() = x;
                IC_curves[i].y.back() = y;
                IC_curves[i].SegLen_x.back() += len_x;
                IC_curves[i].SegLen_y.back() += len_y;
            }
        }
        IC_curves[i].NbEnds = IC_curves[i].x.size();
        IC_curves[i].NbSegs = IC_curves[i].NbEnds - 1;
        IC_curves[i].NbConsegs = 1;
        IC_curves[i].con_index.push_back(0);
        IC_curves[i].con_index.push_back(IC_curves[i].NbSegs);
    }
};

void join(Curve& V, const Curve& raised_V, const std::vector<Curve>& IC_curves){
    int idx_0 = 0;
    int idx_raised = 0; //iterative index ends for the raised_V
    int End_0 = IC_curves[0].NbSegs;
    int End_raised = raised_V.NbSegs;
    //* todo: first join the first IC_curve and raised_V, and get the based_V
    std::vector<double> based_x;
    std::vector<double> based_y;
    std::vector<double> based_slopes;
    based_x.push_back(0);
    based_y.push_back(IC_curves[0].y[0]);
    bool raised_incumbent = false; //indicate whether the incumbent is raised_V
    double k1, k2, x1, y1, x2, y2;
    double temp_x, steep, origin_x;
    while (true){ 
        //cout << "idx_0 = " << idx_0 << endl;
        //cout << "idx_raised = " << idx_raised << endl;
        if (raised_incumbent) { //incumbent segment is raised_V
            //cout << "incumbent = raised" << endl;
            x1 = raised_V.x[idx_raised+1];
            y1 = raised_V.y[idx_raised+1];
            k1 = raised_V.slopes[idx_raised];
            origin_x = x1;
            while (true){ //shared domain
                if (idx_0 == End_0) break;
                if (IC_curves[0].x[idx_0] >= x1) break;
                x2 = IC_curves[0].x[idx_0];
                y2 = IC_curves[0].y[idx_0];
                k2 = IC_curves[0].slopes[idx_0];
                steep = k2 * (x1 - x2) - y1 + y2;
                //cout << "steep = " << steep << endl;
                if (steep > 0){ //steep enough, intersect if IC_curve long enough
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                    if (temp_x > IC_curves[0].x[idx_0+1]) { //not long enough
                        idx_0++;
                    } else { //intersect
                        y1 = k1 * temp_x + y1 - k1 * x1;
                        x1 = temp_x;
                        if (temp_x < IC_curves[0].x[idx_0+1]) {
                            raised_incumbent = false;
                            break;
                        } else {
                            idx_0++;
                        }
                    }
                } else if (steep == 0){ 
                    if (IC_curves[0].x[idx_0+1] < x1) { //not long enough
                        idx_0++;
                        if (k1 == k2){ //overlap
                            y1 = k1 * temp_x + y1 - k1 * x1;
                            x1 = temp_x;
                            break;
                        }
                    } else {
                        if (IC_curves[0].x[idx_0+1] == x1) {
                            idx_0++;
                        }
                        break;
                    }
                } else if (IC_curves[0].y[idx_0+1] <= y1){ //not long enough, incumbent donimates
                    idx_0++;
                } else {
                    break;
                }
            }
            
            if (x1 == origin_x){
                if (based_slopes.empty() || k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(y1);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = y1;
                }
                idx_raised++;
                if (idx_raised == End_raised) break;
            } else if (!raised_incumbent) {
                if (based_slopes.empty() || k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(y1);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = y1;
                }
            }
        }else{ //incumbent segment is IC_curves[0]
            //cout << "incumbent = IC" << endl;
            x1 = IC_curves[0].x[idx_0+1];
            y1 = IC_curves[0].y[idx_0+1];
            k1 = IC_curves[0].slopes[idx_0];
            origin_x = x1;
            while (true) { //shared domain
                if (raised_V.x[idx_raised] >= x1) {
                    //cout << "shared domain" << endl;
                    break;
                }
                x2 = raised_V.x[idx_raised];
                y2 = raised_V.y[idx_raised];
                k2 = raised_V.slopes[idx_raised];
                steep = k2 * (x1 - x2) - y1 + y2;
                //cout << "steep = " << steep << endl;
                if (steep > 0){ //steep enough, intersect if raised_V is long enough
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                    //cout << "temp_x = " << temp_x << endl;
                    if (temp_x > raised_V.x[idx_raised+1]){ // not long enough
                        idx_raised++;
                    } else {
                        y1 = k1 * temp_x + y1 - k1 * x1;
                        x1 = temp_x;
                        if (temp_x < raised_V.x[idx_raised+1]){
                            raised_incumbent = true;
                            //cout << "intersect" << endl;
                            break;
                        } else {
                            idx_raised++;
                        }
                    }
                } else if (steep == 0) {
                    if (raised_V.x[idx_raised+1] < x1) { //not long enough
                        idx_raised++;
                        if (k1 == k2){ //overlap
                            y1 = k1 * temp_x + y1 - k1 * x1;
                            x1 = temp_x;
                            //cout << "overlap" << endl;
                            break;
                        }
                    } else {
                        if (raised_V.x[idx_raised+1] == x1) {
                            idx_raised++;
                        }
                        //cout << "touch right end" << endl;
                        break;
                    }
                } else if (raised_V.y[idx_raised+1] <= y1) { //not steep enough
                    idx_raised++;
                } else {
                    //cout << "flat but long." << endl;
                    break;
                }
            }

            if (x1 == origin_x){
                if (based_slopes.empty() || k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(y1);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = y1;
                }
                idx_0++;
            } else if (raised_incumbent) {
                if (based_slopes.empty() || k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(y1);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = y1;
                }
            }
        }
    }

    //display(based_x, "based_x");
    //display(based_y, "based_y");
    //display(based_slopes, "based_slopes");

    if (IC_curves.size() == 1) {
        V.x = based_x;
        V.y = based_y;
        V.slopes = based_slopes;
        V.NbEnds = V.x.size();
        V.NbSegs = V.slopes.size();
        V.con_index.push_back(0);
        double temp_k = __DBL_MAX__;
        for (int i = 0; i < V.NbSegs; i++){
            V.SegLen_x.push_back(V.x[i+1] - V.x[i]);
            V.SegLen_y.push_back(V.y[i+1] - V.y[i]);
            if (V.slopes[i] > temp_k){
                V.con_index.push_back(i);
            }
            temp_k = V.slopes[i];
        }
        V.con_index.push_back(V.NbSegs);
        V.NbConsegs = V.con_index.size()-1;
        return;
    }

    //todo: then join the based_V with the remaining IC_curves
    V.x.push_back(based_x[0]);
    V.y.push_back(based_y[0]);
    int incumbent = 0; //0 = based; i for IC_curves[i]
    std::vector<int> idx(IC_curves.size(), 0);
    int idx_based = 0;
    int candidate; //record closest intersect and the candidate incumbent
    double incumbent_slope; //used to record k in case of identical intersect
    while(true){       
        // cout << "incumbent = " << incumbent << endl;
        // cout << "idx_based = " << idx_based << endl;
        // cout << "idx[1] = " << idx[1] << endl;
        if (incumbent){//incumbent is the index of IC_curves
            x1 = IC_curves[incumbent].x[idx[incumbent]+1]; //incumbent_sec_x
            y1 = IC_curves[incumbent].y[idx[incumbent]+1]; //incumbent_sec_y
            k1 = IC_curves[incumbent].slopes[idx[incumbent]]; //incumbent_slope
            if (x1 == IC_curves[incumbent].x.back()){ // this is the last 
                incumbent_slope = 0;
            } else {
                incumbent_slope = k1;
            }
            candidate = incumbent; // if after checking all other curves, candidate still = incumbent, 
            origin_x = x1;
            //todo: check based_V, take advantage of its first position
            while (true){ //break when have identified the longest x1 that ensure incumbent dominates the candidate
                if (idx_based == based_slopes.size()) break; //* Break Condition: candidate ends 
                if (based_x[idx_based] >= x1) break; //* Break Condition: candidate and incumbent(x1) have no shared domain
                x2 = based_x[idx_based];
                y2 = based_y[idx_based];
                k2 = based_slopes[idx_based];
                steep = k2 * (x1 - x2) - y1 + y2;
                if (steep > 0){ //steep enough 
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2); //temp_x must <= x1
                    if (temp_x > based_x[idx_based+1]){ // not long enough, idx++ next iteration
                        idx_based++;
                    } else {
                        y1 = k1 * temp_x + y1 - k1 * x1;
                        x1 = temp_x;
                        if (temp_x < based_x[idx_based+1]){
                            incumbent_slope = k2;
                            candidate = 0; //can so far confirm it is a steeper candidate 
                            break; //* Break Condition: interior intersect
                        } else {
                            idx_based++; //go through next iteration 
                        }
                    }
                } else if (steep == 0){ //if touch incumbent, touch its right end
                    if (based_x[idx_based+1] < x1){
                        idx_based++; //*Note: idx_based has changed
                        if (k1 == k2){ //overlap
                            y1 = k1 * based_x[idx_based] + y1 - k1 * x1;
                            x1 = based_x[idx_based];
                            break; //* Break Condition: overlap
                        }
                    } else { // 
                        if (based_x[idx_based+1] == x1){
                            idx_based++;
                        }
                        if (incumbent_slope == 0){ //touch the end of incumbent
                            if (idx_based != based_slopes.size()){ //it can be a candidate
                                incumbent_slope = k2;
                                candidate = 0;
                            }
                        }
                        break; //*Break Condition: candidate is not steep enough but long enough w.r.t x1
                    }
                } else if (based_y[idx_based+1] <= y1){ //not steep and long enough; 
                    ++idx_based; //dominated; let next iteration check break it if necessary
                } else {
                    break; //*Break Condition: candidate is not steep enough but long enough w.r.t y1
                }
            }
            //todo: check other IC_curves
            for (int i = 1; i < IC_curves.size(); i++){
                if (i != incumbent){ //other curves
                    while (true){ //stop when have identified the longest x1 that ensures incumbent(x1) dominates candidate
                        if (idx[i] == IC_curves[i].NbSegs) break; //*Break: candidate ends
                        if (IC_curves[i].x[idx[i]] >= x1) break; //*Break: no shared domain
                        x2 = IC_curves[i].x[idx[i]];
                        y2 = IC_curves[i].y[idx[i]];
                        k2 = IC_curves[i].slopes[idx[i]];
                        steep = k2 * (x1 - x2) - y1 + y2;
                        if (steep > 0){ //steep enough
                            temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                            if (temp_x > IC_curves[i].x[idx[i]+1]){ //not long enough, idx++ next iteration
                                idx[i]++;
                            } else {
                                y1 = k1 * temp_x + y1 - k1 * x1;
                                x1 = temp_x;
                                if (temp_x > IC_curves[i].x[idx[i]+1]){
                                    incumbent_slope = k2;
                                    candidate = i;
                                } else {
                                    idx[i]++;
                                }
                                break; //*Break: intersect
                            }
                        } else if (steep == 0){
                            if (IC_curves[i].x[idx[i]+1] < x1){
                                idx[i]++; //*Note: idx has changed
                                if (k1 == k2){ //overlap
                                    y1 = k1 * IC_curves[i].x[idx[i]] + y1 - k1 * x1;
                                    x1 = IC_curves[i].x[idx[i]];
                                    break; //* Break Condition: overlap
                                }
                            } else { // 
                                if (IC_curves[i].x[idx[i]+1] == x1){
                                    idx[i]++;
                                } 
                                if (incumbent_slope == 0) { //touch the end of incumbent
                                    if (idx_based != based_slopes.size()){ //it can be a candidate
                                        incumbent_slope = k2;
                                        candidate = i;
                                    }
                                } else if (k2 > incumbent_slope){ //pass through a same intersect
                                    incumbent_slope = k2;
                                    candidate = i;
                                }
                                break; //*Break: candidate is not steep enough but long enough w.r.t x1; early domain break
                            }                           
                        } else if (IC_curves[i].y[idx[i]+1] <= y1) { //not long and steep, dominated
                            idx[i]++; //let next iteration break loop
                        } else {
                            break; //*Break: candidate is not steep enough but long enough w.r.t y1;
                        }
                    } 
                }
            }

            if (x1 == origin_x){ //incumbent can be pushed back
                if (V.slopes.empty() || k1 != V.slopes.back()){
                    V.x.push_back(x1);
                    V.y.push_back(y1);
                    V.slopes.push_back(k1);
                } else {
                    V.x.back() = x1;
                    V.y.back() = y1;
                }
                idx[incumbent]++; //if this is the end, candidate will have shifted (steep == 0 case)
                if (x1 == based_x.back()) break;
                if (idx[incumbent] == IC_curves[incumbent].NbEnds) {
                    incumbent = candidate;
                }
            } else if (candidate != incumbent) {
                if (x1 != V.x.back()) {
                    if (V.slopes.empty() || k1 != V.slopes.back()){
                        V.x.push_back(x1);
                        V.y.push_back(y1);
                        V.slopes.push_back(k1);
                    } else {
                        V.x.back() = x1;
                        V.y.back() = y1;
                    }                   
                }
                incumbent = candidate;
            } //else: do nothing
        } else { //incunmbent is the based one
            x1 = based_x[idx_based+1]; //incumbent_sec_x
            y1 = based_y[idx_based+1]; //incumbent_sec_y
            k1 = based_slopes[idx_based]; //incumbent_slope
            incumbent_slope = k1; //incumbent_sec_k
            candidate = incumbent; // 
            origin_x = x1;
            //todo: check from index 1 to IC_curves.size()
            for (int i = 1; i < IC_curves.size(); i++){
                while (true){ //stop when have identified the longest x1 that ensures incumbent(x1) dominates candidate
                    if (idx[i] == IC_curves[i].NbSegs) break; //*Break: candidate ends
                    if (IC_curves[i].x[idx[i]] >= x1) break; //*Break: no shared domain
                    x2 = IC_curves[i].x[idx[i]];
                    y2 = IC_curves[i].y[idx[i]];
                    k2 = IC_curves[i].slopes[idx[i]];
                    steep = k2 * (x1 - x2) - y1 + y2;
                    //cout << "steep = " << steep << endl;
                    if (steep > 0){ //steep enough
                        temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                        if (temp_x > IC_curves[i].x[idx[i]+1]){ //not long enough, idx++ next iteration
                            idx[i]++;
                        } else {
                            y1 = k1 * temp_x + y1 - k1 * x1;
                            x1 = temp_x;
                            if (temp_x > IC_curves[i].x[idx[i]+1]){
                                incumbent_slope = k2;
                                candidate = i;
                            } else {
                                idx[i]++;
                            }
                            break; //*Break: intersect
                        }
                    } else if (steep == 0){
                        if (IC_curves[i].x[idx[i]+1] < x1){
                            idx[i]++; //*Note: idx has changed
                            if (k1 == k2){ //overlap
                                y1 = k1 * IC_curves[i].x[idx[i]] + y1 - k1 * x1;
                                x1 = IC_curves[i].x[idx[i]];
                                break; //* Break Condition: overlap
                            }
                        } else { // 
                            if (IC_curves[i].x[idx[i]+1] == x1){
                                idx[i]++;
                            } else if (x1 != origin_x && k2 > incumbent_slope){ //pass through a same intersect
                                incumbent_slope = k2;
                                candidate = i;
                            }
                            break; //*Break: candidate is not steep enough but long enough w.r.t x1; early domain break
                        }                           
                    } else if (IC_curves[i].y[idx[i]+1] <= y1) { //not long and steep, dominated
                        idx[i]++; //let next iteration break loop
                    } else {
                        break; //*Break: candidate is not steep enough but long enough w.r.t y1;
                    }
                } 
            }

            if (x1 == origin_x){ //incumbent can be pushed back, candidate must == incumbent
                if (V.slopes.empty() || k1 != V.slopes.back()){
                    V.x.push_back(x1);
                    V.y.push_back(y1);
                    V.slopes.push_back(k1);
                } else {
                    V.x.back() = x1;
                    V.y.back() = y1;
                }
                idx_based++;
                if (x1 == based_x.back()) break;
            } else if (candidate != incumbent) {
                if (x1 != V.x.back()) {
                    if (V.slopes.empty() || k1 != V.slopes.back()){
                        V.x.push_back(x1);
                        V.y.push_back(y1);
                        V.slopes.push_back(k1);
                    } else {
                        V.x.back() = x1;
                        V.y.back() = y1;
                    }                   
                }
                incumbent = candidate;
            } //else: do nothing
        }
    }
    V.NbEnds = V.x.size();
    V.NbSegs = V.slopes.size();
    V.con_index.push_back(0);
    double temp_k = __DBL_MAX__;
    for (int i = 0; i < V.NbSegs; i++){
        V.SegLen_x.push_back(V.x[i+1] - V.x[i]);
        V.SegLen_y.push_back(V.y[i+1] - V.y[i]);
        if (V.slopes[i] > temp_k){
            V.con_index.push_back(i);
        }
        temp_k = V.slopes[i];
    }
    V.con_index.push_back(V.NbSegs);
    V.NbConsegs = V.con_index.size()-1;
};

void v2V(Curve& V, const Curve& v){
    V.NbEnds = v.NbEnds;
    V.NbSegs = v.NbSegs;
    V.NbConsegs = v.NbConsegs;
    V.con_index = v.con_index;
    V.x = v.x;
    V.y = v.y;
    V.slopes = v.slopes;
    V.SegLen_x = v.SegLen_x;
    V.SegLen_y = v.SegLen_y;
    double D = v.x[0];
    for (int i = 0; i < V.NbEnds; i++){
        V.x[i] -= D;
    }
}

const double Curve::value_at(const double& point) const{
    //* todo: find value at truncated point
    if (point < 0 || (point - x.back()) > 0.00001){
        cout << "out of range!" << endl;
        cout << "direct misplace = " << std::scientific << std::setprecision(16) << point << endl;
        cout << "x back = " << std::scientific << std::setprecision(16) << x.back() << endl;
        cout << (point - x.back()) << endl;
        this->display();
        throw "out of range";
    } else {
        int i = 0;
        while (true){
            if (x[i] <= point && x[i+1] >= point) {
                return y[i] + slopes[i] * (point - x[i]);
            } else {
                i++;
            }
        }
    }
}

void truncate(Curve& truncated_V, const Curve& V, const Curve& v, const double& gamma){
    double trun_point = v.x[0];
    double sum_C = v.x[1];
    double initial_v = v.y[0];
    //* todo: find value at truncated point
    double initial_V = 0;
    int i = 0;
    while (true) {
        if (V.x[i] <= trun_point && V.x[i+1] >= trun_point){
            if (V.x[i] == trun_point){
                initial_V = V.y[i];
            } else if (V.x[i+1] == trun_point){
                initial_V = V.y[i+1];
                if (++i == V.NbEnds){ //reduce to a single point
                    truncated_V.x.push_back(0);
                    truncated_V.y.push_back(initial_V + initial_v);
                    truncated_V.NbEnds = 1;
                    return;
                }
            } else {
                initial_V = V.x[i] + V.slopes[i] * (trun_point - V.x[i]);
            }
            truncated_V.x.push_back(0);
            truncated_V.y.push_back(initial_V + initial_v);
            truncated_V.slopes.push_back(V.slopes[i] + gamma);
            i++;
            break;
        }
        i++;
    } // exit with i points to the next index of V[x] right after the trun_point

    if (sum_C >= V.x.back() - trun_point){ //all raised
        //* todo: all remaining x's minus truncated point
        //* todo: all remaining slopes + gamma
        //* todo: all remaining y + initial_v + gamma * x
        while (i < V.NbSegs){
            truncated_V.x.push_back(V.x[i] - trun_point);
            truncated_V.slopes.push_back(V.slopes[i] + gamma);
            truncated_V.y.push_back(V.y[i] + initial_v + gamma * truncated_V.x.back());
            i++;
        }
        truncated_V.x.push_back(V.x[i] - trun_point);
        truncated_V.y.push_back(V.y[i] + initial_v + gamma * truncated_V.x.back());
    } else {
        //* todo: before break point all x minus trun_point
        //* todo: add a break point v.x[1] if not exist
        //* todo: after break point all x minus trun_point
        //* todo: before break point, all slope + gamma, all y + initial_value + gamma * x
        //* todo: after break point, all slope remain, all y + initial_value + gamma * break_point
        double break_point = sum_C + trun_point;
        if (V.x[i] > break_point) {
            
        }
        cout << "i = " << i << endl;
        while (V.x[i] < break_point) { // before break point
            truncated_V.x.push_back(V.x[i] - trun_point);
            truncated_V.y.push_back(V.y[i] + initial_v + gamma * truncated_V.x.back());
            truncated_V.slopes.push_back(V.slopes[i] + gamma);
            i++;
        }
        if (V.x[i] == break_point){
            truncated_V.x.push_back(sum_C);
            truncated_V.y.push_back(V.y[i] + initial_v + gamma * sum_C);
            truncated_V.slopes.push_back(V.slopes[i]);
            i++;
        } else {
            truncated_V.y.push_back(truncated_V.y.back() + truncated_V.slopes.back() * (sum_C - truncated_V.x.back()));
            truncated_V.x.push_back(sum_C);
        }
        if (i < V.NbSegs){
            truncated_V.slopes.push_back(V.slopes[i]);
            i++;
            while (i < V.NbSegs){
                truncated_V.x.push_back(V.x[i] - trun_point);
                truncated_V.y.push_back(V.y[i] + initial_v + gamma * sum_C);
                truncated_V.slopes.push_back(V.slopes[i] + gamma);
                i++;
            }
            cout << "i = " << i << endl;
            cout << "V.x[i] = " << V.x[i] << endl;
            truncated_V.x.push_back(V.x[i] - trun_point);
            truncated_V.y.push_back(V.y[i] + initial_v + gamma * sum_C);
        }
    }
    truncated_V.NbEnds = truncated_V.x.size();
    truncated_V.NbSegs = truncated_V.slopes.size();
    truncated_V.con_index.push_back(0);
    double temp_k = __DBL_MAX__;
    for (int i = 0; i < truncated_V.NbSegs; i++){
        truncated_V.SegLen_x.push_back(truncated_V.x[i+1] - truncated_V.x[i]);
        truncated_V.SegLen_y.push_back(truncated_V.y[i+1] - truncated_V.y[i]);
        if (truncated_V.slopes[i] > temp_k){
            truncated_V.con_index.push_back(i);
        }
        temp_k = truncated_V.slopes[i];
    }
    truncated_V.con_index.push_back(truncated_V.NbSegs);
    truncated_V.NbConsegs = truncated_V.con_index.size()-1;
}

void Curve::shift(const double& V){
    auto it = this->y.begin();
    while (it != y.end()){
        *it += V;
        it++;
    }
}