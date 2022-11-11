#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <stdexcept>

double utils_abs(double x);
std::vector<double> operator+(const std::vector<double>& a, const std::vector<double> &b);
std::vector<double> operator*(double a, std::vector<double> v);
#endif