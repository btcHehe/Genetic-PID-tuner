#include "Population.hpp"
#include <iostream>

int main() {
    // destination simulation parameters
    double Tr = 2.0;
    double Os = 0.1;
    double T5s = 7.5;
    Population p(Tr, Os, T5s, 100);
    int iter = 30;     // number of finding iterations

    Sim_params sp;
    p.simulate_population();
    Entity best = p.get_best_member();
    best.simulate(dt, Ts, true, "init_best.csv");
    for (int i=0; i<iter; i++) {
        p.find_solution(76, 0.05, 0.0, 10.0);
        double mean = p.get_mean_adaptation();
        std::cout << "mean adapt: " << mean << std::endl;
        best = p.get_best_member();
        sp = best.simulate(dt, Ts);
        double Tr_error = sp.Tr - Tr;
        double Os_error = sp.Os - Os;
        double T5s_error = sp.T5s - T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        std::cout << "J: " << J << std::endl;
        std::cout << "Tr: " << sp.Tr << ", Os: " << sp.Os << ", T5s: " << sp.T5s << std::endl;
        std::cout << "kp: " << best.get_kp() << ", kd: " << best.get_kd() << ", ki: " << best.get_ki() << std::endl;
        std::cout << "-----" << std::endl;
    }
    best.simulate(dt, Ts, true, "best_found.csv");


    return 0;
}