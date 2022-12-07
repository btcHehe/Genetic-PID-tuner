#include "Population.hpp"

/*
    FILE: Population.cpp
    DESCRIPTION: Class Population method definitions. Methods are implementing activities of genetic algorithm.
*/


/*  -------------------------------------------------------------------------
    |                                                                       |
    |                           PUBLIC METHODS                              |
    |                                                                       |
    ------------------------------------------------------------------------- */

/** @brief - General method for running genetic algorithm. Returns best agent after rep_num algorithm iterations
 * @param rep_num - number of entities to replace
 * @param mutation_chance - probability of mutation happening (default is 10%)
 * @param min_val - minimum random value of mutated variable
 * @param max_val - maximum random value of mutated variable
*/
Entity Population::find_solution(int rep_num, double mutation_chance, double min_val, double max_val) {
    this->simulate_population();
    this->selection();
    this->crossing();
    this->mutation(mutation_chance, min_val, max_val);
    this->replacement(rep_num);
    return this->get_best_member();
}


/** @brief - Method implementing entity selection for reproduction mechanism 
*/
void Population::selection() {
    this->calc_current_adapt_coeff();
    // this->selected_group.clear();
    this->selected_group = this->population;
    // std::vector<double> adapt_meas_vec;     // vector of adaptation measure values
    // std::vector<double> select_prob;        // vector of probabilities of selection
    // std::vector<double> distr_func;         // vector of values of distribution function 
    // double adapt_sum = 0.0;

    // // creation of roulette
    // for (int j=0; j<(int)this->population.size(); j++) {
    //     double f = this->get_adapt_measure(this->population[j]);
    //     adapt_meas_vec.push_back(f);
    //     adapt_sum += f;
    // }
    // for (auto f : adapt_meas_vec) {
    //     select_prob.push_back(f / adapt_sum);
    // }
    // double p_sum = 0.0;
    // for (int k=0; k<(int)this->population.size(); k++) {
    //     p_sum += select_prob[k];
    //     distr_func.push_back(p_sum);
    // }
    // auto roulette_dist = std::uniform_real_distribution<double>(0.0, 1.0);                  // distribution object for roulette
    // // spinning roulette N times
    // for (int n=0; n<(int)this->population.size(); n++) {
    //     double r = roulette_dist(this->rand_gen);
    //     auto first_good = std::upper_bound(distr_func.begin(), distr_func.end(), r);
    //     int index = first_good - distr_func.begin();
    //     this->selected_group.push_back(this->population[index]);
    // }
} /* end of selection() */


/** @brief - Method implementing arithmetic crossing of entities in population; returns children of current population */
void Population::crossing() {
    std::vector<Entity> population_copy = this->selected_group;
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


/** @brief - Method implementing equal-value fenotype mutation of entities in population 
 * @param mutation_chance - probability of mutation happening (default is 10%)
 * @param min_val - minimum random value of mutated variable
 * @param max_val - maximum random value of mutated variable
 */ 
void Population::mutation(double mutation_chance, double min_val, double max_val) {
    this->dist = std::uniform_real_distribution<double>(min_val, max_val);                  // distribution object
    std::uniform_int_distribution<int> param_dist(0, 3*100*mutation_chance - 1);            // 3/(3*100*mutation_chance) -> chance for mutation
    for (int i=0; i<(int)this->children.size(); i++) {
        int rnd_param = param_dist(this->rand_gen);
        switch (rnd_param)
        {
        case 0:                                                                             // mutation of kp
            this->children[i].set_kp(this->dist(rand_gen));
            break;
        case 1:                                                                             // mutation of kd
            this->children[i].set_kd(this->dist(rand_gen));
            break;
        case 2:                                                                             // mutation of ki
            this->children[i].set_ki(this->dist(rand_gen));
            break;
        default:                                                                            // mutation won't happen
            break;
        }
    }
} /* end of mutation() */


/** @brief - Method for replacment some entities of previus population with children 
 * @param rep_num - number of entities to replace
*/
void Population::replacement(int rep_num) {
    // auto rep_dist = std::uniform_int_distribution<int>(0, this->population.size());                     // distribution object
    std::shuffle(this->population.begin(), this->population.end(), this->rand_gen);                     // shuffling entities in population
    std::shuffle(this->children.begin(), this->children.end(), this->rand_gen);                         // shuffling children 
    for (int i=0; i<rep_num; i++) {
        this->population[i] = this->children[i];        // replacing N randomly selected (by shuffling) entities with N children
    }
} /* end of replacement() */


/*  -------------------------------------------------------------------------
    |                                                                       |
    |                           PRIVATE METHODS                             |
    |                                                                       |
    ------------------------------------------------------------------------- */

/** @brief - Method filling vector specimen_evaluation with simulation parameters of every specimen group entity */
void Population::simulate_population() {
    std::vector<Sim_params> v;
    for (int i=0; i<(int)this->population.size(); i++) {
        v.push_back(this->population[i].simulate(dt, Ts));
    }
    this->specimen_evaluation = v;
} /* end of simulate_population() */


/** @brief - Method for finding best entity in population 
 */
Entity Population::get_best_member() {
    double min_error = 1000000.0;
    int min_error_index = 0;

    for (int i=0; i<(int)this->population.size(); i++) {
        double Tr_error = this->specimen_evaluation[i].Tr - this->goal_parameters.Tr;
        double Os_error = this->specimen_evaluation[i].Os - this->goal_parameters.Os;
        double T5s_error = this->specimen_evaluation[i].T5s - this->goal_parameters.T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;

        if (J < min_error) {
            min_error_index = i;
            min_error = J;
        }
    }
    return this->population[min_error_index];
} /* end of get_best() */


/** @brief - Method for calculation mean adaptation measure value 
*/
double Population::get_mean_adaptation() {
    double mean_J = 0.0;
    for (int i=0; i<(int)this->population.size(); i++) {
        double Tr_error = this->specimen_evaluation[i].Tr - this->goal_parameters.Tr;
        double Os_error = this->specimen_evaluation[i].Os - this->goal_parameters.Os;
        double T5s_error = this->specimen_evaluation[i].T5s - this->goal_parameters.T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        mean_J += J;
    }
    mean_J /= this->population.size();
    return mean_J;
} /* end of get_mean_adaptation() */


/** @brief - Method for calculation of adaptation measure of agent for current population 
 * @param agent - member of population for which the adaptation measure have to be calculated
 */
double Population::get_adapt_measure(Entity agent) {
    Sim_params agent_sim = agent.simulate(this->time_step, this->time_sim);
    double Tr_error = agent_sim.Tr - this->goal_parameters.Tr;
    double Os_error = agent_sim.Os - this->goal_parameters.Os;
    double T5s_error = agent_sim.T5s - this->goal_parameters.T5s;
    double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
    return this->adapt_coeff - J;            // adaptation measure for given agent of current populationj
} /* end of get_adapt_measure() */


/** @brief - Method for getting adaptation coefficient for current population */
void Population::calc_current_adapt_coeff() {
    Entity best_mem = this->get_best_member();
    Sim_params best_sim = best_mem.simulate(this->time_step, this->time_sim);
    double Tr_error = best_sim.Tr - this->goal_parameters.Tr;
    double Os_error = best_sim.Os - this->goal_parameters.Os;
    double T5s_error = best_sim.T5s - this->goal_parameters.T5s;
    double Cmax = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
    this->adapt_coeff = Cmax;
} /* end of calc_current_adapt_ceoff() */