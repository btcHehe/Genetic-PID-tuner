#include "Population.hpp"
#include <iostream>

int main() {
    // destination simulation parameters
    double Tr = 2.0;
    double Os = 0.1;
    double T5s = 7.5;
    Population p(Tr, Os, T5s, 50);
    int iter = 100;     // number of finding iterations

    std::vector<double> mean_cost;
    std::vector<double> best_cost;

    Sim_params sp;
    p.simulate_population();
    Entity best = p.get_best_member();
    best.simulate(dt, Ts, true, "init_best.csv");
    for (int i=0; i<iter; i++) {
        p.find_solution(40, 0.1, 0.0, 5.0);
        double mean = p.get_mean_adaptation();
        std::cout << "mean adapt: " << mean << std::endl;
        mean_cost.push_back(mean);
        best = p.get_best_member();
        sp = best.simulate(dt, Ts);
        double Tr_error = sp.Tr - Tr;
        double Os_error = sp.Os - Os;
        double T5s_error = sp.T5s - T5s;
        double J = ((Tr_error * Tr_error) + (Os_error * Os_error) + (T5s_error * T5s_error)) / 3;
        best_cost.push_back(J);
        std::cout << "J: " << J << std::endl;
        std::cout << "Tr: " << sp.Tr << ", Os: " << sp.Os << ", T5s: " << sp.T5s << std::endl;
        std::cout << "kp: " << best.get_kp() << ", kd: " << best.get_kd() << ", ki: " << best.get_ki() << std::endl;
        std::cout << "-----" << std::endl;
    }
    best.simulate(dt, Ts, true, "best_found.csv");

    std::ofstream file;
    file.open("mean_cost.csv");
    for (int i=0; i<(int)mean_cost.size(); i++) {
        file << mean_cost[i] << ",";
    }
    file.close();

    file.open("best_cost.csv");
    for (int i=0; i<(int)best_cost.size(); i++) {
        file << best_cost[i] << ",";
    }
    file.close();

    return 0;
}