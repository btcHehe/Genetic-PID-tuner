#include "Entity.hpp"

/*
    FILE: Entity.cpp
    DESCRIPTION: Class Entity method definitions. Methods are implementing simulation of dynamic system, stability check and other usefull actions.
*/

/*  -------------------------------------------------------------------------
    |                                                                       |
    |                           PUBLIC METHODS                              |
    |                                                                       |
    ------------------------------------------------------------------------- */

/** @brief - step response analysis using bilinear transform method 
 * @param sample_t - sampling time (timestep of simulation) [s]
 * @param sim_t - simulation duration time [s]
 * @param save_results - flag for saving step response results to csv file
*/
Sim_params Entity::simulate(double sample_t, double sim_t, bool save_results, std::string filename) {
    Sim_params result_params;
    double T = sample_t;
    int N = sim_t/T;                                // number of simulation steps
    std::vector<double> y; 
    std::vector<double> u;
    std::vector<double> r;

    for (int j=0; j<INIT_SAMPLE_N; j++) {
        y.push_back(0.0);                           // initial conditions of output (y[n-2], y[n-1])
        u.push_back(0.0);                           // initial conditions of input (u[n-2], u[n-1])
        r.push_back(0.0);                           // initial conditions of input (r[n-2], r[n-1])
    }

    // difference equation coefficients
    double u0 = 2*T*this->a1 + T*T*this->a0;
    double u1 = 2*T*T*this->a0;
    double u2 = T*T*this->a0 - 2*T*this->a1;

    double y0 = T*T*this->b0 + 2*T*this->b1 + 4*this->b2;
    double y1 = 2*T*T*this->b0 - 8*this->b2;
    double y2 = T*T*this->b0 - 2*T*this->b1 + 4*this->b2;

    double e0 = 4*this->kd + 2*T*this->kp + T*T*this->ki;
    double e1 = 2*T*T*this->ki - 8*this->kd;
    double e2 = 4*this->kd + T*T*this->ki - 2*T*this->kp;

    double div = y0 + u0*e0/(2*T);

    double max_val = 0.0;               // value for finding overshoot
    bool tr_flag = true;                // latch flag for finding Tr
    bool tr_flag_10perc = true;         // latch flag for finding 10% reach time
    int tr_10perc = 0.0;                // time when step response reached 10%

    // simulation loop
    for (int n=INIT_SAMPLE_N; n<N+INIT_SAMPLE_N; n++) {
        r.push_back(1.0);               // unit step input
        double y_n = (
            u[n-2]*(u2 + u0) +
            u[n-1]*u1 +
            r[n-2]*e2*u0/(2*T) +
            r[n-1]*e1*u0/(2*T) + 
            r[n]*e0*u0/(2*T) -
            y[n-2]*(y2 + u0*e2/(2*T)) -
            y[n-1]*(y1 + u0*e1/(2*T))
        ) / div;
        y.push_back(y_n);

        double u_n = (
            2*T*u[n-2] +
            r[n-2]*e2 +
            r[n-1]*e1 +
            r[n]*e0 -
            y[n-2]*e2 -
            y[n-1]*e1 -
            y[n]*e0
        ) / (2*T);
        u.push_back(u_n);

        if (y_n >= 0.1 && tr_flag_10perc) {                         // 10 % rise time latch
            tr_10perc = (n-INIT_SAMPLE_N)*T;                        // n-INIT_SAMPLE_N because of INIT_SAMPLE_N initial samples
            tr_flag_10perc = false;
        }
        if (y_n >= 0.9 && tr_flag) {                                // rise time detection latch
            result_params.Tr = ((n-INIT_SAMPLE_N)*T) - tr_10perc;   // 90% rise time - 10% rise time
            tr_flag = false;
        }
        if (max_val < y_n) {                                        // updating maximum value so far
            max_val = y_n;
        }
    }

    // T5s search
    for (int i=(int)y.size(); i>0; i--) {
        if (abs(y[i] - 1.0) <= 0.05) {
            result_params.T5s = (i-INIT_SAMPLE_N)*T;
        } else {
            break;
        }
    }

    // Overshoot
    result_params.Os = max_val - 1;

    // optional response saving for debug
    if (save_results)
        this->save_sim(y, filename);

    return result_params;
} /* end of simulate() */


/** @brief - save simulation results to .csv file
* @param  y - output vector to be saved
* @param filename - csv file name
*/
void Entity::save_sim(std::vector<double> y, std::string filename) {
    std::ofstream file;
    file.open(filename);
    for (int i=0; i<(int)y.size(); i++) {
        file << y[i] << ",";
    }
    file.close();
    return;
} /* end of save_sim() */


/** @brief - method for checking if such controller parameters destabilize system 
*/
bool Entity::is_stable() {
    double p1, p2, p3;          // system transfer function denominator coefficients: s^3 + p1*s^2 + p2*s + p3
    double p0 = (this->b2 + this->a1*this->kd);
    p1 = (this->b1 + this->a0*this->kd + this->a1*this->kp)/p0;
    p2 = (this->b0 + this->a0*this->kp + this->a1*this->ki)/p0;
    p3 = this->a0*this->ki/p0;
    gsl_complex r1, r2, r3;     // complex roots of system transfer function denominator polynomial
    gsl_poly_complex_solve_cubic(p1, p2, p3, &r1, &r2, &r3);
    if (GSL_REAL(r1) >= 0.0 || GSL_REAL(r2) >= 0.0 || GSL_REAL(r3) >= 0.0) {   // system to be stable require all roots to be on left half of complex plane
        return false;
    } else {
        return true;
    }
} /* end of is_stable() */


/* ------------------- Getters and setters --------------------- */
void Entity::set_kp(double new_kp) {
    this->kp = new_kp;
}

void Entity::set_kd(double new_kd) {
    this->kd = new_kd;
}

void Entity::set_ki(double new_ki) {
    this->ki = new_ki;
}

double Entity::get_kp() {
    return this->kp;
}

double Entity::get_kd() {
    return this->kd;
}

double Entity::get_ki() {
    return this->ki;
}
