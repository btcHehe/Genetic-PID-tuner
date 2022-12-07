#include "Population.hpp"
#include <iostream>

int main() {
    // destination simulation parameters
    double Tr = 10;
    double Os = 10;
    double T5s = 10;
    Population p(Tr, Os, T5s, 50);
    int iter = 20;     // number of finding iterations

    // p.simulate_population();
    // Entity best_init = p.get_best_member(Tr, Os, T5s);
    // best_init.simulate(dt, Ts, true, "init_best.csv");

    for (int i=0; i<iter; i++) {
        p.find_solution(25, 0.1);
        double mean = p.get_mean_adaptation();
        std::cout << "mean adapt: " << mean << std::endl;
        Entity best = p.get_best_member();
        Sim_params sp = best.simulate(dt, Ts);
        double Tr_error = sp.Tr - Tr;
        double Os_error = sp.Os - Os;
        double T5s_error = sp.T5s - T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        std::cout << "J: " << J << std::endl;
        std::cout << "Tr: " << sp.Tr << ", Os: " << sp.Os << ", T5s: " << sp.T5s << std::endl;
        std::cout << "-----" << std::endl;
    }
    // p.simulate_population();
    // Entity best_found = p.get_best_member(Tr, Os, T5s);
    // best_found.simulate(dt, Ts, true, "best_found.csv");

    return 0;
}