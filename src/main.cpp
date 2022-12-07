#include "Population.hpp"
#include <iostream>

int main() {
    Population p(50);
    int iter = 100;     // number of finding iterations

    // destination simulation parameters
    double Tr = 1;
    double Os = 1;
    double T5s = 1;

    p.simulate_population();
    Entity best_init = p.get_best_member(Tr, Os, T5s);
    best_init.simulate(dt, Ts, true, "init_best.csv");

    for (int i=0; i<iter; i++) {
        p.crossing();
        p.mutation(0.01);
        p.selection();
        p.simulate_population();
        Entity best = p.get_best_member(Tr, Os, T5s);
        Sim_params sp = best.simulate(dt, Ts);
        double Tr_error = sp.Tr - Tr;
        double Os_error = sp.Os - Os;
        double T5s_error = sp.T5s - T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        std::cout << "J: " << J << std::endl;
        // std::cout << "Tr: " << sp.Tr << ", Os: " << sp.Os << ", T5s: " << sp.T5s << std::endl;
    }
    p.simulate_population();
    Entity best_found = p.get_best_member(Tr, Os, T5s);
    best_found.simulate(dt, Ts, true, "best_found.csv");

    return 0;
}