#ifndef SIM_PARAMS_HPP
#define SIM_PARAMS_HPP

/*! Structure containing simulation result parameters */
struct Sim_params
{
    double Tr;          // rise time
    double Os;          // overshoot
    double T5s;         // 5% settling time
};

#endif