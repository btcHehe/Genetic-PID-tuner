#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <vector>
#include "Entity.hpp"

/*! Class implementing population of entities */
class Population {
    public:
        /** @brief - Contructor of class Population; Initializes N entities with random parameters
         * @param N - number of entities to create in population
         */
        Population(int N) {

        } /* end of Population() */

        /** @brief - Method implementing arithmetic crossing of entities in population */
        void crossing();

        /** @brief - Method implementing equal-value fenotype mutation of entities in population */
        void mutation();
    private:
        std::vector<Entity> specimen_group;         // group of entities
};

#endif 