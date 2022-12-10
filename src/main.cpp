#include "Population.hpp"
#include <iostream>

int main() {
    // destination simulation parameters
    double Tr = 2.4;
    double Os = 0.3;
    double T5s = 9.5;
    int population_size = 100;

    double max_kp = 20.0;
    double max_kd = 1.0;
    double max_ki = 4.0;

    Population p(Tr, Os, T5s, population_size, max_kp, max_kd, max_ki, dt, Ts);
    int iter = 60;     // number of finding iterations

    std::vector<double> mean_cost;
    std::vector<double> best_cost;

    p.simulate_population();
    Entity best = p.specimen_evaluation[p.get_best_member()];
    best.simulate(dt, Ts, true, "init_best.csv");
    for (int i=0; i<iter; i++) {
        double J = p.find_solution(60, 0.2);
        double mean = p.get_mean_adaptation();
        std::cout << "mean adapt: " << mean << std::endl;
        mean_cost.push_back(mean);
        // best = p.get_best_member();
        // double J = p.get_cost(best);
        // sp = best.simulate(dt, Ts);
        best_cost.push_back(J);
        std::cout << "J: " << J << std::endl;
        // std::cout << "Tr: " << sp.Tr << ", Os: " << sp.Os << ", T5s: " << sp.T5s << std::endl;
        // std::cout << "kp: " << best.get_kp() << ", kd: " << best.get_kd() << ", ki: " << best.get_ki() << std::endl;
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