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
    //the first end of the raised curve
    double raised_x = v.x[v.NbSegs] - v.x[0];
    double raised_y = v.y[v.NbSegs] + V.y[0];
    double raised_slope = 0;
    raised_V.x.push_back(raised_x);
    raised_V.y.push_back(raised_y);
    raised_V.con_index.push_back(0);

    double Dt = v.x[0]; //the threshold Dt
    int idx = 0;
    while (idx < V.NbEnds - 1){ //
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
    raised_V.NbConsegs = raised_V.con_index.size() - 1;
};

void inf_convolute(std::vector<Curve>& IC_curves, const Curve& V, const Curve& v){
    int End_v = v.x.back(); //last end index of v
    for (int i = 0; i < V.NbConsegs; i++){ //for each concave segment
        int idx_v = 0; //iterative index of ends for v
        int idx_V = V.con_index[i]; //iterative index of ends for V's concave segment i
        int End_V = V.con_index[i+1]; //final end index of V's concave segment i
        double x = V.x[idx_V]; //starting x of IC_curve
        double y = v.y[0]+V.y[i]; //starting y of IC_curve
        IC_curves[i].x.push_back(x); //push back the starting x
        IC_curves[i].y.push_back(y); //push back the starting y
        //*slope re-arrange
        double len_x, len_y, slope_to_add;
        while (idx_v < End_v || idx_V < End_V){ //there's still slope to insert
            if (idx_v == End_v || V.slopes[idx_V] >= v.slopes[idx_v]){ //insert next slope from V
                len_x = V.SegLen_x[idx_V];
                len_y = V.SegLen_y[idx_V];
                slope_to_add = V.slopes[idx_V];
                ++idx_V;
            } else { //insert next slope from v
                len_x = v.SegLen_x[idx_v];
                len_y = v.SegLen_y[idx_v];
                slope_to_add = v.slopes[idx_v];
                ++idx_v;
            }
            x += len_x; //next x value
            y += len_y; //next y value
            if (IC_curves[i].slopes.back() != slope_to_add) { //add a new segment
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
    std::vector<int> idx(IC_curves.size(), 0); //iterative index of ends for each IC_curve
    int idx_0 = 0;
    int idx_raised = 0; //iterative index ends for the raised_V
    int End_0 = IC_curves[0].NbSegs;
    int End_raised = raised_V.NbSegs;
    //* todo: first join the first IC_curve and raised_V, and get the based_V
    std::vector<double> based_x;
    std::vector<double> based_y;
    std::vector<double> based_slopes;
    double incumbent_y = IC_curves[0].y[0];
    double incumbent_x = IC_curves[0].x[0];
    based_x.push_back(0);
    based_y.push_back(incumbent_y);
    based_slopes.push_back(IC_curves[0].slopes[0]);
    bool raised_incumbent = false; //indicate whether the incumbent is raised_V
    double k1, k2, x1, y1, x2, y2;
    double temp_x, temp_y, temp_slope, steep;
    while (idx_0 < End_0){ 
        if (raised_incumbent) { //incumbent segment is raised_V
            // while (idx_0 < End_0 && IC_curves[0].y[idx_0+1] <= incumbent_y){ //!maybe redundant
            //     ++idx_0;
            // }
            //if (idx_0 == End_0) break;
            x1 = raised_V.x[idx_raised+1]; //!can be placed to right after where idx_raised updates
            y1 = raised_V.y[idx_raised+1];
            k1 = raised_V.slopes[idx_raised];
            if (IC_curves[0].x[idx_0] < raised_V.x[idx_raised+1]){ //shared domain
                x2 = IC_curves[0].x[idx_0];
                y2 = IC_curves[0].y[idx_0];
                k2 = IC_curves[0].slopes[idx_0];
                steep = (y1 - y2) - k2 * (x1 - x2);
                if (steep > 0){ //steep enough, intersect if IC_curve long enough
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                    if (temp_x < IC_curves[0].x[idx_0+1]){ // IC_curve is long enough
                        //* todo: compute intersect point, record an incumbent y, 
                        raised_incumbent = false;
                        if (temp_x != based_x.back()){ // push back
                            //* todo: push back the confirmed section (check extension).
                            incumbent_x = temp_x;
                            incumbent_y = k1*incumbent_x + y1 - k1*x1;
                            if (k1 != based_slopes.back()){
                                based_x.push_back(incumbent_x);
                                based_y.push_back(incumbent_y);
                                based_slopes.push_back(k1);
                            } else {
                                based_x.back() = incumbent_x;
                                based_y.back() = incumbent_y;
                            } 
                        }
                    } else { // IC_curve is not long enough, being dominated
                        ++idx_0;
                    }
                } else if (steep == 0){ //
                    if (IC_curves[0].x[idx_0+1] >= x1){
                        //* todo: push back incumbent
                        if (k1 != based_slopes.back()){
                            based_x.push_back(x1);
                            based_y.push_back(y1);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = x1;
                            based_y.back() = y1;
                        }
                        idx_raised++;
                    }
                    if (IC_curves[0].x[idx_0+1] <= x1){ 
                        idx_0++; //dominated.
                    }
                } else { //not steep enough
                    if (IC_curves[0].x[idx_0+1] >= x1){ //long enough to push back incumbent
                        incumbent_y = y1;
                        if (k1 != based_slopes.back()){
                            based_x.push_back(x1);
                            based_y.push_back(incumbent_y);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = x1;
                            based_y.back() = incumbent_y;
                        }
                        ++idx_raised;
                    }
                    if (IC_curves[0].y[idx_0+1] <= y1){ //not long enough, incumbent donimates
                        ++idx_0;
                    } 
                }
            } else { //no shared domain (idx_raised can ++)
                //* todo: push back the incumbent segment, update incumbent y (extension check)
                incumbent_y = y1;
                if (k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(incumbent_y);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = incumbent_y;
                }
                ++idx_raised;
            }
        }else{ //incumbent segment is IC_curves[0]
            // while (raised_V.y[idx_raised+1] <= incumbent_y){
            //     ++idx_raised;
            // }
            //if (idx_raised == End_raised) break; //* impossible
            x1 = IC_curves[0].x[idx_0+1];
            y1 = IC_curves[0].y[idx_0+1];
            k1 = IC_curves[0].slopes[idx_0];
            if (raised_V.x[idx_raised] < IC_curves[0].x[idx_0+1]){ //shared domain
                x2 = raised_V.x[idx_raised];
                y2 = raised_V.y[idx_raised];
                k2 = raised_V.slopes[idx_raised];
                steep = y1 - y2 - k2 * (x1 - x2);
                if (steep > 0){ //steep enough, intersect if raised_V is long enough
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                    if (temp_x < raised_V.x[idx_raised+1]){ //raised_V is long enough
                        //* todo: compute intersect point, record an incumbent y, 
                        raised_incumbent = true;
                        if (temp_x != based_x.back()){ //swap
                            incumbent_x = temp_x;
                            incumbent_y = k1*incumbent_x + y1 - k1*x1;
                            //* todo: else push back the confirmed section (check extension).
                            if (k1 != based_slopes.back()){
                                based_x.push_back(incumbent_x);
                                based_y.push_back(incumbent_y);
                                based_slopes.push_back(k1);
                            } else {
                                based_x.back() = incumbent_x;
                                based_y.back() = incumbent_y;
                            }                             
                        }
                    } else { //not long enough, will not intersect with this raised_V segment
                        ++idx_raised;
                    }  
                } else if (steep == 0) {
                    if (raised_V.x[idx_raised+1] >= x1){
                        //* todo: push back incumbent
                        if (k1 != based_slopes.back()){
                            based_x.push_back(x1);
                            based_y.push_back(y1);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = x1;
                            based_y.back() = y1;
                        }
                        idx_0++;
                    }
                    if (IC_curves[0].x[idx_0+1] <= x1){
                        idx_raised++;
                    }
                } else { //not steep enough
                    if (raised_V.x[idx_raised+1] >= x1){ //long enough to push back incumbent
                        incumbent_y = y1;
                        if (k1 != based_slopes.back()){
                            based_x.push_back(x1);
                            based_y.push_back(incumbent_y);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = x1;
                            based_y.back() = incumbent_y;
                        }
                        ++idx_0;
                    }
                    if (raised_V.y[idx_raised+1] <= y1){ //not long enough, incumbent donimates
                        ++idx_raised;
                    } 
                }
            } else { //no shared domain
                //* todo: push back the incumbent segment, update incumbent y (extension check)
                incumbent_y = y1;
                if (k1 != based_slopes.back()){
                    based_x.push_back(x1);
                    based_y.push_back(incumbent_y);
                    based_slopes.push_back(k1);
                } else {
                    based_x.back() = x1;
                    based_y.back() = incumbent_y;
                }
                ++idx_0;
            }
        }
    }
    if (idx_raised < End_raised){ //there are still segments in raised_V
        if (raised_V.slopes[idx_raised] != based_slopes.back()){ //extension check
            based_x.push_back(raised_V.x[idx_raised+1]);
            based_y.push_back(raised_V.y[idx_raised+1]);
            based_slopes.push_back(raised_V.slopes[idx_raised]);
        } else {
            based_x.back() = raised_V.x[idx_raised+1];
            based_y.back() = raised_V.y[idx_raised+1];
        }
        ++idx_raised;
    }
    while (idx_raised < End_raised){ // must have idx_0 == End_0
        //* todo: add all remaining raised_V segments
        based_x.push_back(raised_V.x[idx_raised+1]);
        based_y.push_back(raised_V.y[idx_raised+1]);
        based_slopes.push_back(raised_V.slopes[idx_raised]);
        ++idx_raised;
    }

    //todo: then join the based_V with the remaining IC_curves
    V.x[0] = based_x[0];
    V.y[0] = based_y[0];
    int incumbent = 0; //0 = based; i for IC_curves[i]
    std::vector<int> idx(IC_curves.size(), 0);
    int idx_based = 0;
    int candidate; //record closest intersect and the candidate incumbent
    double incumbent_slope, origin_x; //used to record k in case of identical intersect
    bool pass; //true if all segments from other curves have no intersect and can not increment, in this case idx_incumbent should ++
    while(true){
        pass = true;
        if (incumbent){//incumbent is the index of IC_curves
            x1 = IC_curves[incumbent].x[idx[incumbent]+1]; //incumbent_sec_x
            y1 = IC_curves[incumbent].y[idx[incumbent]+1]; //incumbent_sec_y
            k1 = IC_curves[incumbent].slopes[idx[incumbent]]; //incumbent_slope
            incumbent_slope = k1; //incumbent_sec_k
            candidate = incumbent; // if after checking all other curves, candidate still = incumbent, 
            origin_x = x1;
            //todo: check based_V, take advantage of its first position
            while (true){ //shared domain
                based_x[idx_based] < x1;
                x2 = based_x[idx_based];
                y2 = based_y[idx_based];
                k2 = based_slopes[idx_based];
                steep = y1 - y2 - k2 * (x1 - x2);
                if (steep > 0){ //steep enough 
                    pass = false;
                    temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2); //temp_x must <= x1
                    if (temp_x < based_x[idx_based+1]) { //long enough, exist unique interior intersect
                        //* todo: calculate intersect, update candidate, incumbent slope
                        x1 = temp_x; //can only ensure dominance within temp_x
                        incumbent_slope = k2;
                        candidate = 0;
                        break;
                        //because based is the first to check, the logic will be simpler
                    } else { //steep enough but not long enough, pass this segment
                        ++idx_based; //when = holds, no need to change candidate
                        if (temp_x == based_x[idx_based+1]){
                            break;
                        }
                    }
                } else if (steep == 0){
                    if (based_x[idx_based+1] <= x1){
                        idx_based++;
                        if (k1 == k2){ //overlap
                            x1 = based_x[idx_based+1];
                            candidate = 0;
                            pass = false;
                        }
                    }
                } else if (based_y[idx_based+1] <= y1){ //not steep and long enough
                    ++idx_based; //dominated
                    if (based_x[idx_based+1] < x1){
                        pass = false; //the next based segment may intersect, should not pass incumbent
                    }
                } //not steep enough but long enough: pass remains.
            }
            //todo: check other IC_curves
            for (int i = 1; i < IC_curves.size(); i++){
                if (i != incumbent){ //other curves
                    if (IC_curves[i].x[idx[i]] < x1){ //shared domain
                        x2 = IC_curves[i].x[idx[i]];
                        y2 = IC_curves[i].y[idx[i]];
                        k2 = IC_curves[i].slopes[idx[i]];
                        if (y1 - y2 >= k2 * (x1 - x2)){ //steep enough
                            pass = false;
                            if (k1 != k2) {//not overlap
                                temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                                if (temp_x < IC_curves[i].x[idx[i]+1]){ //long enough, unique intersect exists
                                    if (temp_x < x1){ //closer than x1
                                        x1 = temp_x;
                                        if (temp_x != V.x.back() || k2 > incumbent_slope){ //closer interior intersect for incumbent or steeper left-ended intersect
                                            incumbent_slope = k2;
                                            candidate = i;
                                        }
                                    } else if (k2 > incumbent_slope) { //as close but steeper
                                        incumbent_slope = k2;
                                        candidate = i;
                                    } // else (same distanced intersect && flatter slope)
                                } else { //steep enough but not long enough, pass this segment
                                    ++idx[i];
                                }
                            } else { //overlap
                                //* todo: intersect is the min of (x1, IC_curve[i].x), no need to update incumbent slope
                                candidate = i;
                                if (x1 >= IC_curves[i].x[idx[i]+1]){ //IC_curve_x is shorter, update x1
                                    x1 = IC_curves[i].x[idx[i]+1];
                                    ++idx[i]; //this segment is dominated
                                }
                            }
                        } else if (IC_curves[i].x[idx[i]+1] <= x1) { //not long and steep, dominated
                            idx[i]++;
                            pass = false;
                        }
                    } else { //no shared domained
                        //* todo: do nothing
                    }
                }
            }

            if (x1 == origin_x){ //the original incumbent is dominating
                if (k1 != V.slopes.back()){
                    V.x.push_back(x1);
                    V.y.push_back(y1);
                    V.slopes.push_back(k1);
                } else {
                    V.x.back() = x1;
                    V.y.back() = y1;
                }
            } else { //the original segment is cut, only dominate within x1, there must be a candidate
                if (x1 != V.x.back()){ 
                    //* todo: push back the incumbent x and y and slope (check extension)
                    y1 = k1*x1 + y1 - k1*x1;
                    if (k1 != V.slopes.back()){
                        V.x.push_back(x1);
                        V.y.push_back(y1);
                        V.slopes.push_back(k1);
                    } else {
                        V.x.back() = x1;
                        V.y.back() = y1;
                    }
                }
                incumbent = candidate;
            }
            
            if (pass){ //all segments either (no shared domain) or (have shared domain but all are not steep but long enough)
                //* todo: push back the incumbent and increment idx[incumbent] (check extension)
                if (k1 != V.slopes.back()){
                    V.x.push_back(x1);
                    V.y.push_back(y1);
                    V.slopes.push_back(k1);
                } else {
                    V.x.back() = x1;
                    V.y.back() = y1;
                }
                idx[incumbent]++;
            } else if (candidate != incumbent) { //shared domain, and has intersect; i.e. exist one segment steep and long enough
                //* todo: update the incumbent
                incumbent = candidate;
                if (x1 != V.x.back()){ 
                    //* todo: push back the incumbent x and y and slope (check extension)
                    y1 = k1*x1 + y1 - k1*x1;
                    if (k1 != V.slopes.back()){
                        V.x.push_back(x1);
                        V.y.push_back(y1);
                        V.slopes.push_back(k1);
                    } else {
                        V.x.back() = x1;
                        V.y.back() = y1;
                    }
                } 
            } else { //shared domain but no intersect, i.e., all not long enough
                //* todo: nothing, idx_incumbent should not ++
            }


        } else { //incunmbent is the based one
            //todo: check from index 1 to IC_curves.size()
            for (int i = 1; i < IC_curves.size(); i++){
                
            }
        }
    }
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
