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
*/
double Population::find_solution(int rep_num, double mutation_chance) {
    this->simulate_population();
    // std::cout << "Population: ";
    // for (int i=0; i<(int)this->population.size(); i++) {
    //     std::cout << "J:" << this->specimen_evaluation[i] << " kp:" << this->population[i].get_kp() << " kd:" << this->population[i].get_kd() << " ki:" << this->population[i].get_ki() <<std::endl;
    // }
    std::cout << std::endl;
    this->selection();
    this->crossing();
    this->mutation(mutation_chance);
    this->replacement(rep_num);
    return this->specimen_evaluation[this->get_best_member()];
}


/** @brief - Method implementing entity selection for reproduction mechanism 
*/
void Population::selection() {
    this->selected_group.clear();
    this->simulate_population();

    std::vector<double> adapt_meas_vec;     // vector of adaptation measure values
    std::vector<double> select_prob;        // vector of probabilities of selection
    std::vector<double> distr_func;         // vector of values of distribution function 
    double adapt_sum = 0.0;

    // creation of roulette
    for (int j=0; j<(int)this->population.size(); j++) {
        double f = this->specimen_evaluation[j];
        adapt_meas_vec.push_back(f);
        adapt_sum += f;
    }
    for (auto f : adapt_meas_vec) {
        select_prob.push_back(f * adapt_sum);
    }
    double p_sum = 0.0;
    for (int k=0; k<(int)this->population.size(); k++) {
        p_sum += select_prob[k];
        distr_func.push_back(p_sum);
    }
    auto roulette_dist = std::uniform_real_distribution<double>(0.0, 1.0);                  // distribution object for roulette
    // spinning roulette N times
    for (int n=0; n<(int)this->population.size(); n++) {
        double r = roulette_dist(this->rand_gen);
        auto first_good = std::upper_bound(distr_func.begin(), distr_func.end(), r);
        int index = first_good - distr_func.begin();
        this->selected_group.push_back(this->population[index]);
    }
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

    // std::cout << "cost\n";
    // for (auto c: this->children) {
    //     std::cout << this->get_cost(c) << " ";
    // }
    std::cout << std::endl;
} /* end of crossing() */


/** @brief - Method implementing equal-value fenotype mutation of entities in population 
 * @param mutation_chance - probability of mutation happening (default is 10%)
 */ 
void Population::mutation(double mutation_chance) {
    std::uniform_int_distribution<int> param_dist(0, 3*100*mutation_chance - 1);            // 3/(3*100*mutation_chance) -> chance for mutation
    for (int i=0; i<(int)this->children.size(); i++) {
        int rnd_param = param_dist(this->rand_gen);
        switch (rnd_param)
        {
        case 0:                                                                             // mutation of kp
            this->children[i].set_kp(this->dist_kp(rand_gen));
            break;
        case 1:                                                                             // mutation of kd
            this->children[i].set_kd(this->dist_kd(rand_gen));
            break;
        case 2:                                                                             // mutation of ki
            this->children[i].set_ki(this->dist_ki(rand_gen));
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
    // this->simulate_population();
    std::vector<double> cost_v = this->specimen_evaluation;

    std::vector<size_t> idx(this->specimen_evaluation.size());
    std::iota(idx.begin(), idx.end(), 0);       // idx contains indexes from 0 to N

    // std::cout << "\npre: \n";
    // for (int i=0; i<(int)this->specimen_evaluation.size(); i++) {
    //     std::cout << idx[i] << " ";
    // }

    // sorting indexes in idx corresponding to cost values (from smallest cost to highest)
    std::stable_sort(idx.begin(), idx.end(), [&cost_v](size_t i1, size_t i2) {return cost_v[i1] > cost_v[i2];});

    // std::cout << "\npost: \n";
    // for (int i=0; i<(int)this->specimen_evaluation.size(); i++) {
    //     std::cout << idx[i] << "," << this->specimen_evaluation[idx[i]] << " ";
    // }

    for (int i=0; i<rep_num; i++) {
        int weak_idx = idx[i];
        this->population[weak_idx] = this->children[i];        // replacing N randomly selected (by shuffling) entities with N children
    }
} /* end of replacement() */


/*  -------------------------------------------------------------------------
    |                                                                       |
    |                           PRIVATE METHODS                             |
    |                                                                       |
    ------------------------------------------------------------------------- */

/** @brief - Method filling vector specimen_evaluation with simulation parameters of every specimen group entity */
void Population::simulate_population() {
    this->specimen_evaluation.clear();
    std::vector<double> v_cost;
    for (int i=0; i<(int)this->population.size(); i++) {
        v_cost.push_back(this->get_cost(this->population[i]));
    }

    this->specimen_evaluation = v_cost;
} /* end of simulate_population() */


/** @brief - Method for finding best entity in population 
 */
int Population::get_best_member() {
    // this->simulate_population();
    double min_error = 1000000.0;
    int min_index = 0;

    for (int i=0; i<(int)this->population.size(); i++) {
        double J = this->specimen_evaluation[i];
        if (J < min_error) {
            min_error = J;
            min_index = i;
        }
    }
    return min_index;
} /* end of get_best() */


/** @brief - Method for calculation mean adaptation measure value 
*/
double Population::get_mean_adaptation() {
    double mean_J = 0.0;
    int N = 0;
    for (int i=0; i<(int)this->population.size(); i++) {
        double J = this->specimen_evaluation[i];
        if (J == UNSTABLE_COST)
            continue;
        mean_J += J;
        N++;
    }
    mean_J /= N;
    return mean_J;
} /* end of get_mean_adaptation() */


/** @brief - Method for calculating cost value for agent
 * @param agent - member of population
*/
double Population::get_cost(Entity agent) {
    Sim_params agent_sim = agent.simulate(this->time_step, this->time_sim);
    double Tr_error;
    double Os_error = agent_sim.Os - this->goal_parameters.Os;
    double T5s_error;

    // if (agent_sim.Tr < dt) {
        // Tr_error = UNSTABLE_COST;
    // } else 
        Tr_error = agent_sim.Tr - this->goal_parameters.Tr;

    // if (agent_sim.T5s < dt) {
        // T5s_error = UNSTABLE_COST;
    // } else 
        T5s_error = agent_sim.T5s - this->goal_parameters.T5s;
    
    if (!agent.is_stable()) {
        return UNSTABLE_COST;
    } else {
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        return J;
    }
}