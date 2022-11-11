#include "utils.hpp"

double utils_abs(double x) {
    if (x < 0.0) {
        return -1.0 * x;
    } else {
        return x;
    }
}

std::vector<double> operator+(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() == b.size()) {
        std::vector<double> result;
        for (int i=0; i<(int)a.size(); i++) {
            result.push_back(a[i] + b[i]);
        }
        return result;
    } else {
        throw std::invalid_argument("Added vectors must be of equal size.");
    }
}

std::vector<double> operator*(double a, std::vector<double> v) {
    std::vector<double> result;
    for (int i=0; i<(int)v.size(); i++) {
        result.push_back(v[i]*a);
    }
    return result;
}