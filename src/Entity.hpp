#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sim_params.hpp"
#include <vector>

/*! Class implementing specimen of the population of solutions */
class Entity {
    public:
        /** @brief - step response analysis using bilinear transform method 
         * @param sample_t - sampling time (timestep of simulation) [s]
         * @param sim_t - simulation duration time [s]
        */
        Sim_params simulate(double sample_t, double sim_t);
    private:
        double kp;                  // proportional gain
        double kd;                  // differential gain
        double ki;                  // integral gain
};

#endif