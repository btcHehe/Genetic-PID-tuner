#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sim_params.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <complex.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_poly.h>

/*! Class implementing specimen of the population of solutions */
class Entity {
    public:
        /** @brief - Entity class constuctor; initializes controller parameters */
        Entity(double k_p=1.0, double k_d=1.0, double k_i=1.0) {
            this->kp = k_p;
            this->kd = k_d;
            this->ki = k_i;
        } /* end of Entity() */

        /** @brief - step response analysis using bilinear transform method 
         * @param sample_t - sampling time (timestep of simulation) [s]
         * @param sim_t - simulation duration time [s]
         * @param save_results - flag for saving step response results to csv file
        */
        Sim_params simulate(double sample_t, double sim_t, bool save_results=false);

        /** @brief - save simulation results to .csv file
         * @param  y - output vector to be saved
         * @param filename - csv file name
        */
        void save_sim(std::vector<double> y, std::string filename);

        /** @brief - method for checking if such controller parameters destabilize system 
         * @param k_p - proposed proportional gain
         * @param k_d - proposed derivative gain
         * @param k_i - proposed integral gain
        */
        bool is_stable(double k_p, double k_d, double k_i);

        /* Getters and setters */
        void set_kp(double new_kp);
        double get_kp();
        void set_kd(double new_kd);
        double get_kd();
        void set_ki(double new_ki);
        double get_ki();
    private:
        double kp;                  // proportional gain
        double kd;                  // differential gain
        double ki;                  // integral gain
        // plant transfer function parameters
        double a0 = 1.24;
        double a1 = 2.61;
        double b0 = 1.2;
        double b1 = 2.11;
        double b2 = 4.17;
};

#endif