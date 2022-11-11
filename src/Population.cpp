#include "Population.hpp"

/** @brief - Method implementing entity selection mechanism */
void Population::selection() {

} /* end of selection() */


/** @brief - Method implementing arithmetic crossing of entities in population; returns children of current population */
void Population::crossing() {
    std::vector<Entity> population_copy = this->specimen_group;
    std::vector<Entity> pair_0;                                             // vector containing first entities of the pair
    std::vector<Entity> pair_1;                                             // vector containing second entities of the pair
    std::uniform_int_distribution<int> int_dist0(0, (int)population_copy.size());
    std::uniform_int_distribution<int> int_dist1(0, (int) population_copy.size());
    while (! population_copy.empty()) {                                      // creating random pairs
        // first pair member
        int_dist0 = std::uniform_int_distribution<int>(0, (int)population_copy.size()-1);
        int p0_index = int_dist0(this->rand_gen);
        pair_0.push_back(population_copy[p0_index]);                        // save random entity
        population_copy.erase(population_copy.begin() + p0_index);          // remove randomly selected entity from the pool
        // second pair member
        int_dist1 = std::uniform_int_distribution<int>(0, (int)population_copy.size()-1);
        int p1_index = int_dist1(this->rand_gen);
        pair_1.push_back(population_copy[p1_index]);                        // save random entity
        population_copy.erase(population_copy.begin() + p1_index);          // remove randomly selected entity from the pool
    }
    // performing arithmetic crossing
    std::uniform_real_distribution<double> a_dist(0.0, 1.0);                // distribution object for selecting random value of "a" crossing parameter
    double a = 0.0;                                                         // parameter of arithmetic crossing
    for (int i=0; i<(int)pair_0.size(); i++) {
        a = a_dist(this->rand_gen);
        std::vector<double> params_p0 = {pair_0[i].get_kp(), pair_0[i].get_kd(), pair_0[i].get_ki()};
        std::vector<double> params_p1 = {pair_1[i].get_kp(), pair_1[i].get_kd(), pair_1[i].get_ki()};
        std::vector<double> p0_child = a*params_p0 + (1.0 - a)*params_p1;
        std::vector<double> p1_child = a*params_p1 + (1.0 - a)*params_p0;
        this->children[i] = Entity(p0_child[0], p0_child[1], p0_child[2]);
        this->children[i+1] = Entity(p1_child[0], p1_child[1], p1_child[2]);
    }
} /* end of crossing() */


/** @brief - Method implementing equal-value fenotype mutation of entities in population */
void Population::mutation() {

} /* end of mutation() */
