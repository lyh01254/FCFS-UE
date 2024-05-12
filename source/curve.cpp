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
    double temp_x, temp_y, temp_slope;
    while (true){ 
        if (raised_incumbent) { //incumbent segment is raised_V
            while (idx_0 < End_0 && IC_curves[0].y[idx_0+1] <= incumbent_y){
                ++idx_0;
            }
            if (idx_0 == End_0) break;
            if (IC_curves[0].x[idx_0] < raised_V.x[idx_raised+1]){ //shared domain
                x2 = IC_curves[0].x[idx_0];
                y2 = IC_curves[0].y[idx_0];
                k2 = IC_curves[0].slopes[idx_0];
                x1 = raised_V.x[idx_raised+1]; //!can be placed to right after where idx_raised updates
                y1 = raised_V.y[idx_raised+1];
                k1 = raised_V.slopes[idx_raised];
                if (y1 - y2 >= k2 * (x1 - x2)){ //steep enough, intersect if IC_curve long enough
                    if (k1 != k2){ //not overlap, there is a unique itersect (if any)
                        temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                        if (temp_x <= IC_curves[0].x[idx_0+1]){ // IC_curve is long enough
                            //* todo: compute intersect point, record an incumbent y, 
                            if (temp_x == based_x.back()){ // swap but no push back
                                raised_incumbent = false;
                            } else { // keep incumbent
                                //* todo: else push back the confirmed section (check extension).
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
                        } else { // IC_curve is not long enough, will not intersect with this IC_curve segment
                            ++idx_0;
                        }
                    } else {// the two segments overlap partially
                        //* todo: push back the shorter segment, update incumbent y. (check extension)
                        if (x1 <= IC_curves[0].x[idx_0+1]){
                            incumbent_y = y1;
                            incumbent_x = x1;
                            ++idx_raised;
                        } else {
                            incumbent_x = IC_curves[0].x[idx_0+1];
                            incumbent_y = IC_curves[0].y[idx_0+1];
                            ++idx_0;
                        }
                        if (k1 != based_slopes.back()){
                            based_x.push_back(incumbent_x);
                            based_y.push_back(incumbent_y);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = incumbent_x;
                            based_y.back() = incumbent_y;
                        }
                    }
                } else { //not steep enough
                    if (IC_curves[0].x[idx_0+1] < x1){ // not long enough, incumbent donimates
                        ++idx_0;
                    } else { //not steep enough but long enough to push back incumbent, incumbent ++
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
            while (idx_raised < End_raised && raised_V.y[idx_raised+1] <= incumbent_y){
                ++idx_raised;
            }
            //if (idx_raised == End_raised) break; //* impossible
            if (raised_V.x[idx_raised] < IC_curves[0].x[idx_0+1]){ //shared domain
                x1 = IC_curves[0].x[idx_0+1];
                y1 = IC_curves[0].y[idx_0+1];
                k1 = IC_curves[0].slopes[idx_0];
                x2 = raised_V.x[idx_raised];
                y2 = raised_V.y[idx_raised];
                k2 = raised_V.slopes[idx_raised];
                if (y1 - y2 >= k2 * (x1 - x2)){ //steep enough, intersect if raised_V is long enough
                    if (k1 != k2){
                        temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                        if (temp_x <= raised_V.x[idx_raised+1]){ //raised_V is long enough
                            //* todo: compute intersect point, record an incumbent y, 
                            if (temp_x == based_x.back()){ //swap
                                raised_incumbent = true;
                            }else { //keep incumbent
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
                    } else {// the two segments overlap partially
                        //* todo: push back the shorter segment, update incumbent y. (check extension).
                        if (x1 <= raised_V.x[idx_raised+1]){
                            incumbent_x = x1;
                            incumbent_y = y1;
                            ++idx_0;
                        } else {
                            incumbent_x = raised_V.x[idx_raised+1];
                            incumbent_y = raised_V.y[idx_raised+1];
                            idx_raised++;
                        }
                        if (k1 != based_slopes.back()){
                            based_x.push_back(incumbent_x);
                            based_y.push_back(incumbent_y);
                            based_slopes.push_back(k1);
                        } else {
                            based_x.back() = incumbent_x;
                            based_y.back() = incumbent_y;
                        }                        
                    }
                } else { //not steep enough
                    if (raised_V.x[idx_raised+1] < x1){ //not long enough, dominated by incumbent
                        ++idx_raised;
                    } else { // not steep enough but long enough to push back incumbent
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
    double incumbent_slope; //used to record k in case of identical intersect
    bool pass; //true if all segments from other curves have no intersect and can not increment, in this case idx_incumbent should ++
    while(true){
        pass = true;
        if (incumbent){//incumbent is the index of IC_curves
            x1 = IC_curves[incumbent].x[idx[incumbent]+1];
            y1 = IC_curves[incumbent].y[idx[incumbent]+1];
            k1 = IC_curves[incumbent].slopes[idx[incumbent]];
            incumbent_x = x1;
            incumbent_y = y1;
            incumbent_slope = k1;
            candidate = incumbent; // if after checking all other curves, candidate still = incumbent, 
            //todo: check based_V
            if (based_x[idx_based] < x1){ //shared domain
                x2 = based_x[idx_based];
                y2 = based_y[idx_based];
                k2 = based_slopes[idx_based];
                if (y1 - y2 >= k2 * (x1 - x2)){ //steep enough //!maybe should change the benchmark
                    pass = false;
                    if (k1 != k2){ //not overlap
                        temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                        if (temp_x <= based_x[idx_based+1]) { //long enough
                            //* todo: calculate intersect and record it as temp_x, temp_y, temp_slope, candidate
                            if (temp_x != V.x.back()){ //interior intersect for incumbent
                                if (temp_x < incumbent_x){ //if temp_x is closer 
                                    //* todo: update incumbent x, y and slope, record candidate
                                    incumbent_x = temp_x;
                                    incumbent_y = k1*temp_x + y1 - k1*x1;
                                    incumbent_slope = k2; 
                                    candidate = 0;
                                } else if (temp_x == incumbent_x) { //if temp_x is as close as incumbent x
                                    //* todo: choose the steeper one, update candidate, incumbent slope
                                    if (k2 > incumbent_slope){
                                        incumbent_slope = k2;
                                        candidate = 0;
                                    }
                                } // if temp_x
                            } else { //intersect is the left end of incumbent, must have k2 > incumbent_slope
                                //* todo: update candidate, incumbent x and slope
                                incumbent_x = temp_x;
                                incumbent_slope = k2;
                                candidate = 0;
                            }  
                        } else { //steep enough but not long enough, pass this segment
                            ++idx_based;
                        }
                    } else { //!overlap
                        //* todo: intersect is the min of (x1, based_V[idx_based+1]), no need to update incumbent slope
                        if (x1 >= based_x[idx_based+1]){ 
                            incumbent_x = based_x[idx_based+1];
                            incumbent_y = based_y[idx_based+1];
                            ++idx_based;
                        } else {
                            incumbent_x = x1;
                            incumbent_y = y1;
                            candidate = 0;
                        }
                    }
                } else if (based_x[idx_based+1] <= x1){ //not steep and long enough
                    ++idx_based;
                    pass = false; //!if the steep benchmark is changed, this should change as well
                } //not steep enough but long enough: pass remains.
            } else { //the segment of this curve has no shared domain
                //* todo: pass remains its value, no need to update anything
            }
            //todo: check other IC_curves
            for (int i = 1; i < IC_curves.size(); i++){
                if (i != incumbent){ //other curves
                    if (IC_curves[i].x[idx[i]] < x1){ //shared domain
                        pass = false;
                        x2 = IC_curves[i].x[idx[i]];
                        y2 = IC_curves[i].y[idx[i]];
                        k2 = IC_curves[i].slopes[idx[i]];
                        if (y1 - y2 >= k2 * (x1 - x2)){ //steep enough
                            if (k1 != k2) {//not overlap
                                temp_x = (k1*x1 - k2*x2 +y2 - y1) / (k1-k2);
                                if (temp_x != V.x.back()){
                                    if (temp_x <= IC_curves[i].x[idx[i]+1]){ //long enough
                                        if (temp_x < incumbent_x){
                                            incumbent_x = temp_x;
                                            
                                        } else if (temp_x == incumbent_x){ //must be temp_x == incumbent_x

                                        } 
                                    } else { //not long enough

                                    }
                                }
                            } else { //overlap

                            }
                            //todo: calculate the intersect and update temp_x, temp_y, candidate
                        } else if (IC_curves[i].x[idx[i]+1] <= x1) { //not long and steep, dominated
                            //todo: pass
                            idx[i]++;
                        }
                    } else { //no shared domained
                        //todo: pass
                    }
                }
            }
            if (pass){ //no segment has shared domain with incumbent
                idx[incumbent]++;
            } else if (candidate != incumbent) { //there are some segments has shared domain with incumbent, and some has intersect
                //todo: update the incumbent,
                if (incumbent_y != V.y.back()){
                    //todo: push back the incumbent x and y and slope (check extension)
                } 
            } else { //shared domain but no intersect
                //todo: nothing, idx_incumbent should not ++
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
