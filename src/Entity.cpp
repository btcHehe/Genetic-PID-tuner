#include "Entity.hpp"
#include "utils.hpp"

/** @brief - step response analysis using bilinear transform method 
 * @param sample_t - sampling time (timestep of simulation) [s]
 * @param sim_t - simulation duration time [s]
*/
Sim_params Entity::simulate(double sample_t, double sim_t) {
    Sim_params result_params;
    double T = sample_t;
    int N = sim_t/T;                        // number of simulation steps
    std::vector<double> y = {0, 0, 0};      // initial conditions of output (y[n-3], y[n-2], y[n-1])
    std::vector<double> u = {0, 0, 0};      // initial conditions of input (u[n-3], u[n-2], u[n-1])

    // object transfer function parameters
    double a0 = 1.0;
    double a1 = 1.0;
    double b0 = 1.0;
    double b1 = 1.0;
    double b2 = 1.0;

    // difference equation coefficients
    double e1 = 8*b2 + 8*a1*this->kd;
    double e2 = 4*T*(b1 + a0*this->kd + a1*this->kp);
    double e3 = 2*T*T*(b0 + a0*this->kp + a1*this->ki);
    double c1 = 8*a1*this->kd;
    double c2 = 4*T*(a0*this->kd + a1*this->kp);
    double c3 = 2*T*T*a0*this->ki;

    double max_val = 0.0;
    bool tr_flag = true;
    std::vector<double> near_1_steps;   // moments when y_n was approximately equal to 1.0

    // simulation loop
    for (int n=3; n<N+3; n++) {
        u.push_back(1.0);
        double y_n = (
            u[n]*(c1 + c2 + c3) - 
            u[n-1]*(3*c1 + c2 - c3) + 
            u[n-2]*(3*c1 - c2 - c3) -
            u[n-3]*(c1 - c2 - c3) + 
            y[n-1]*(3*e1 + e2 - e3) -
            y[n-2]*(3*e1 - e2 - e3) + 
            y[n-3]*(e1 - e2 - e3)
            ) / (e1 + e2 + e3);
        y.push_back(y_n);
        if (y_n >= 1.0 && tr_flag) {    // rise time detection latch
            result_params.Tr = n*T;
            tr_flag = false;
        }
        if (max_val < y_n) {            // updating maximum value so far
            max_val = y_n;
        }
        if (abs(y_n - 1.0) <= 0.01) {   // save moment when response was ~equal to 1.0
            near_1_steps.push_back(n);
        }
    }

    result_params.Os = max_val - 1;

    // T5s search

    return result_params;
} /* end of simulate() */