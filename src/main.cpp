#include "Population.hpp"

int main() {
    // destination simulation parameters
    double Tr = 1.0;
    double Os = 0.1;
    double T5s = 10.0;
    int population_size = 100;

    double max_kp = 10.0;
    double max_kd = 1.0;
    double max_ki = 2.0;

    Population p(Tr, Os, T5s, population_size, max_kp, max_kd, max_ki, dt, Ts);
    int iter = 50;     // number of finding iterations

    std::vector<double> mean_cost;
    std::vector<double> best_cost;
    std::vector<double> kp_v;
    std::vector<double> kd_v;
    std::vector<double> ki_v;

    p.simulate_population();
    for (int i=0; i<iter; i++) {
        double J = p.find_solution(60, 0.2);
        double mean = p.get_mean_adaptation();
        // std::cout << "mean adapt: " << mean << std::endl;
        mean_cost.push_back(mean);
    }

    std::ofstream file;
    file.open("mean_cost.csv");
    for (int i=0; i<(int)mean_cost.size(); i++) {
        file << mean_cost[i] << ",";
    }
    file.close();

    return 0;
}