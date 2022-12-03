#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <vector>
#include <random>
#include <gsl/gsl_vector.h>
#include <iostream>
#include "Entity.hpp"
#include "utils.hpp"

/*! Class implementing population of entities */
class Population {
    public:
        /** @brief - Contructor of class Population; Initializes N entities with random parameters
         * @param N - number of entities to create in population (must be even because of corssing)
         * @param max_random - maximum number that can be generated by rng
         */
        Population(int N=50, double max_random=100.0) {
            // error checking
            if (N%2 != 0) {
                throw std::invalid_argument("Odd size of population was set. Even required.");
            }
            this->dist = std::uniform_real_distribution<double>(0.0, max_random);   // initializing distribution object
            // creating entities with random parameters
            for (int i=0; i<N; i++) {
                Entity tmpE;
                tmpE.set_kp(this->dist(this->rand_gen));
                tmpE.set_kd(this->dist(this->rand_gen));
                tmpE.set_ki(this->dist(this->rand_gen));
                this->specimen_group.push_back(tmpE);
                this->children.push_back(tmpE);                                     // at the beginning children does not matter; filling for indexing purposes
            }
        } /* end of Population() */

        /** @brief - Method implementing entity selection mechanism */
        void selection();

        /** @brief - Method implementing arithmetic crossing of entities in population; returns children of current population */
        void crossing();

        /** @brief - Method implementing equal-value fenotype mutation of entities in population
         * @param mutation_chance - probability of mutation happening (default is 10%)
         * @param min_val - minimum random value of mutated variable
         * @param max_val - maximum random value of mutated variable
         */
        void mutation(double mutation_chance, double min_val=0.0, double max_val=50.0);

        /** @brief - Method filling vector specimen_evaluation with simulation parameters of every specimen group entity */
        void simulate_population();

        /** @brief - Method for finding best entity in population 
        * @param Tr_goal - rise time goal value
        * @param Os_goal - overshoot goal value
        * @param T5s_goal - 5% settling time goal value
        */
        Entity get_best_member(double Tr_goal, double Os_goal, double T5s_goal);
    // private:
        std::vector<Entity> specimen_group;                         // group of entities
        std::vector<Entity> children;                               // current population children
        std::vector<Sim_params> specimen_evaluation;                // vector of simulation results for every specimen_group member
        std::default_random_engine rand_gen;                        // random entity parameters generator
        std::uniform_real_distribution<double> dist;                // distribution object
};

#endif